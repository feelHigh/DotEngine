#include "pch.h"
#include "DCollisionMgr.h"

// Manager Headers
#include "DAssetMgr.h"
#include "DLevelMgr.h"

// Level Headers
#include "DLevel.h"
#include "DLayer.h"

// GameObject Headers
#include "DGameObject.h"

// Component Headers
#include "DCollider2D.h"

DCollisionMgr::DCollisionMgr()
	: m_Matrix{}
{
}

DCollisionMgr::~DCollisionMgr()
{
}

void DCollisionMgr::Tick()
{
	for (int Row = 0; Row < MAX_LAYER; ++Row)
	{
		for (int Col = Row; Col < MAX_LAYER; ++Col)
		{
			if (m_Matrix[Row] & (1 << Col))
			{
				CollisionBtwLayer(Row, Col);
			}
		}
	}
}

void DCollisionMgr::CollisionCheck(UINT Layer1, UINT Layer2)
{
	UINT Row = Layer1;
	UINT Col = Layer2;

	if (Row > Col)
	{
		Row = (UINT)Layer2;
		Col = (UINT)Layer1;
	}

	if (m_Matrix[Row] & (1 << Col))
	{
		m_Matrix[Row] &= ~(1 << Col);
	}
	else
	{
		m_Matrix[Row] |= (1 << Col);
	}
}

void DCollisionMgr::CollisionBtwLayer(UINT _Left, UINT _Right)
{
	// Get information of the current level
	DLevel* pCurLevel = DLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	// Import all objects belonging to each layer
	const vector<DGameObject*>& vecLeft = pCurLevel->GetLayer(_Left)->GetObjects();
	const vector<DGameObject*>& vecRight = pCurLevel->GetLayer(_Right)->GetObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		DCollider2D* pLeftCol = vecLeft[i]->Collider2D();
		if (nullptr == pLeftCol)
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			DCollider2D* pRightCol = vecRight[j]->Collider2D();
			if (nullptr == pRightCol)
				continue;

			COLLIDER_ID id = {};

			id.LeftID = pLeftCol->GetID();
			id.RightID = pRightCol->GetID();

			map<ULONGLONG, bool>::iterator iter = m_mapCollisionInfo.find(id.ID);

			// If it has never been registered, register it
			if (iter == m_mapCollisionInfo.end())
			{
				m_mapCollisionInfo.insert(make_pair(id.ID, false));
				iter = m_mapCollisionInfo.find(id.ID);
			}

			bool bDead = pLeftCol->GetOwner()->IsDead() || pRightCol->GetOwner()->IsDead();
			//bool bDeactive = !vecLeft[i]->IsActive() || !vecRight[j]->IsActive();

			// The two colliders are now colliding.
			if (IsCollision(pLeftCol, pRightCol))
			{
				// Was during colliding
				if (iter->second)
				{
					pLeftCol->Overlap(pRightCol);
					pRightCol->Overlap(pLeftCol);
				}

				// Was not colliding
				else
				{
					pLeftCol->BeginOverlap(pRightCol);
					pRightCol->BeginOverlap(pLeftCol);
				}

				iter->second = true;

				// If either of the collisions is dead or disabled
				// Additionally, call the release of the collision.
				if (bDead)
				{
					pLeftCol->EndOverlap(pRightCol);
					pRightCol->EndOverlap(pLeftCol);
					iter->second = false;
				}
			}

			// The two colliders are not colliding right now.
			else
			{
				// Was during colliding
				if (iter->second)
				{
					pLeftCol->EndOverlap(pRightCol);
					pRightCol->EndOverlap(pLeftCol);
				}

				iter->second = false;
			}
		}
	}
}

bool DCollisionMgr::IsCollision(DCollider2D* _Left, DCollider2D* _Right)
{
	// Gets the basic circular shape of the collider.
	Ptr<DMesh> pRectMesh = DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh");
	tVtx* pVtx = (tVtx*)pRectMesh->GetVtxSysMem();

	// Gets the world matrix of each collider
	const Matrix& matLeft = _Left->GetWorldMat();
	const Matrix& matRight = _Right->GetWorldMat();

	// After multiplying the basic figure (Rect) by the world matrix of each collision object, move it to the position of each corner of the collision object,
	// By subtracting the position values between the coordinates, the surface direction vector of the figure is obtained at the position on the world of the collision body.
	// This vector will be the axis on which the colliders will be projected
	Vec3 vProjAxis[4] = {};

	vProjAxis[0] = XMVector3TransformCoord(pVtx[3].vPos, matLeft) - XMVector3TransformCoord(pVtx[0].vPos, matLeft);
	vProjAxis[1] = XMVector3TransformCoord(pVtx[1].vPos, matLeft) - XMVector3TransformCoord(pVtx[0].vPos, matLeft);

	vProjAxis[2] = XMVector3TransformCoord(pVtx[3].vPos, matRight) - XMVector3TransformCoord(pVtx[0].vPos, matRight);
	vProjAxis[3] = XMVector3TransformCoord(pVtx[1].vPos, matRight) - XMVector3TransformCoord(pVtx[0].vPos, matRight);

	// Vector connecting the center of a collision object
	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matLeft) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matRight);

	// Projection
	for (int i = 0; i < 4; ++i)
	{
		Vec3 vProj = vProjAxis[i];
		vProj.Normalize();

		float dot = fabs(vProjAxis[0].Dot(vProj));
		dot += fabs(vProjAxis[1].Dot(vProj));
		dot += fabs(vProjAxis[2].Dot(vProj));
		dot += fabs(vProjAxis[3].Dot(vProj));
		dot /= 2.f;

		float fCenter = fabs(vCenter.Dot(vProj));

		if (dot < fCenter)
			return false;
	}

	return true;
}