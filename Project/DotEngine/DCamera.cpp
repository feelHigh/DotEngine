#include "pch.h"
#include "DCamera.h"

// Device Headers
#include "DDevice.h"

// Manager Headers
#include "DRenderMgr.h"
#include "DLevelMgr.h"
#include "DTimeMgr.h"
#include "DKeyMgr.h"
#include "DAssetMgr.h"

// Level Headers
#include "DLevel.h"
#include "DLayer.h"

// GameObject Headers
#include "DGameObject.h"

// Component Headers
#include "DTransform.h"
#include "DRenderComponent.h"

DCamera::DCamera()
	: DComponent(COMPONENT_TYPE::CAMERA)
	, m_Priority(-1)
	, m_LayerCheck(0)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_Width(0)
	, m_Height(0)
	, m_Far(100000.f)
	, m_FOV(XM_PI / 2.f)
	, m_ProjectionScale(1.f)
{
	Vec2 vResolution = DDevice::GetInst()->GetResolution();
	m_Width = vResolution.x;
	m_Height = vResolution.y;
	m_AspectRatio = m_Width / m_Height;
}

DCamera::~DCamera()
{
}

void DCamera::Begin()
{
	// Register Camera
	if (-1 != m_Priority)
	{
		DRenderMgr::GetInst()->RegisterCamera(this, m_Priority);
	}
}

void DCamera::FinalTick()
{
	// View Space 란 카메라가 좌표계의 기준이 되는 좌표계
	// 1. Camera is at the origin
	// 2. The camera is looking at the Z-axis

	// 1. Find the coordinates of objects based on where the camera is located at the origin.
	// 2. The camera should turn the direction it was looking at from the world on the Z axis.
	// Objects rotate together.

	// Calculate the View matrix.
	// The matrix used to change the View matrix to World Space -> View Space
	Matrix matTrans = XMMatrixTranslation(-Transform()->GetRelativePos().x, -Transform()->GetRelativePos().y, -Transform()->GetRelativePos().z);

	Matrix matRot;
	Vec3 vR = Transform()->GetWorldDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR::FRONT);

	matRot._11 = vR.x; matRot._12 = vU.x; matRot._13 = vF.x;
	matRot._21 = vR.y; matRot._22 = vU.y; matRot._23 = vF.y;
	matRot._31 = vR.z; matRot._32 = vU.z; matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// Projection Space Projection Coordinate System (NDC)
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 1. Orthographic
		// Projection in a straight line
		// Compressing the field of view to NDC
		m_matProj = XMMatrixOrthographicLH(m_Width * m_ProjectionScale, m_Height * m_ProjectionScale, 1.f, m_Far);
	}

	else
	{
		// 2. Perspective
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
}

void DCamera::SortGameObject()
{
	DLevel* pLevel = DLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (false == (m_LayerCheck & (1 << i)))
		{
			continue;
		}

		DLayer* pLayer = pLevel->GetLayer(i);

		const vector<DGameObject*>& vecObjects = pLayer->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (nullptr == vecObjects[j]->GetRenderComponent()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMesh()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader())
			{
				continue;
			}

			Ptr<DGraphicShader> pShader = vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader();
			SHADER_DOMAIN Domain = pShader->GetDomain();

			switch (Domain)
			{
			case DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case DOMAIN_MASKED:
				m_vecMasked.push_back(vecObjects[j]);
				break;
			case DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case DOMAIN_PARTICLE:
				m_vecParticles.push_back(vecObjects[j]);
				break;
			case DOMAIN_EFFECT:
				m_vecEffect.push_back(vecObjects[j]);
				break;
			case DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			case DOMAIN_UI:
				m_vecUI.push_back(vecObjects[j]);
				break;
			}
		}
	}
}

void DCamera::Render()
{
	// Sort Object
	SortGameObject();

	// View, Proj matrix to use when objects are rendered
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	// Opaque
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}

	// Masked
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->Render();
	}

	// Transparent
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}

	// Particles
	for (size_t i = 0; i < m_vecParticles.size(); ++i)
	{
		m_vecParticles[i]->Render();
	}

	Render_Effect();

	// PostProcess 
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		DRenderMgr::GetInst()->PostProcessCopy();
		m_vecPostProcess[i]->Render();
	}

	// UI
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->Render();
	}

	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecEffect.clear();
	m_vecParticles.clear();
	m_vecPostProcess.clear();
	m_vecUI.clear();
}

void DCamera::Render_Effect()
{
	// Change Render Target
	Ptr<DTexture> pEffectTarget = DAssetMgr::GetInst()->FindAsset<DTexture>(L"EffectTargetTex");
	Ptr<DTexture> pEffectDepth = DAssetMgr::GetInst()->FindAsset<DTexture>(L"EffectDepthStencilTex");

	// Clear Render Target
	CONTEXT->ClearRenderTargetView(pEffectTarget->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 0.f));
	CONTEXT->ClearDepthStencilView(pEffectDepth->GetDSV().Get(), D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH, 1.f, 0);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<FLOAT>(pEffectTarget->GetWidth());
	viewport.Height = static_cast<FLOAT>(pEffectTarget->GetHeight());
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pEffectTarget->GetRTV().GetAddressOf(), pEffectDepth->GetDSV().Get());

	// Effect
	for (size_t i = 0; i < m_vecEffect.size(); ++i)
	{
		m_vecEffect[i]->Render();
	}

	// Change to BlurTarget
	Ptr<DTexture> pEffectBlurTarget = DAssetMgr::GetInst()->FindAsset<DTexture>(L"EffectBlurTargetTex");
	Ptr<DMaterial> pBlurMtrl = DAssetMgr::GetInst()->FindAsset<DMaterial>(L"BlurMtrl");
	Ptr<DMesh> pRectMesh = DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh");

	CONTEXT->ClearRenderTargetView(pEffectBlurTarget->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 0.f));

	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pEffectBlurTarget->GetRTV().GetAddressOf(), nullptr);

	pBlurMtrl->SetTexParam(TEX_0, pEffectTarget);
	pBlurMtrl->Binding();
	pRectMesh->Render_Particle(2);


	// Revert to original Render Target
	Ptr<DTexture> pRTTex = DAssetMgr::GetInst()->FindAsset<DTexture>(L"RenderTargetTex");
	Ptr<DTexture> pDSTex = DAssetMgr::GetInst()->FindAsset<DTexture>(L"DepthStencilTex");
	Ptr<DMaterial> pEffectMergeMtrl = DAssetMgr::GetInst()->FindAsset<DMaterial>(L"EffectMergeMtrl");

	viewport.Width = static_cast<FLOAT>(pRTTex->GetWidth());
	viewport.Height = static_cast<FLOAT>(pRTTex->GetHeight());
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	pEffectMergeMtrl->SetTexParam(TEX_0, pEffectTarget);
	pEffectMergeMtrl->SetTexParam(TEX_1, pEffectBlurTarget);
	pEffectMergeMtrl->Binding();
	pRectMesh->Render();
}

void DCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_Priority, sizeof(int), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Height, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_ProjectionScale, sizeof(float), 1, _File);
}

void DCamera::LoadFromFile(FILE* _File)
{
	fread(&m_Priority, sizeof(int), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Height, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_ProjectionScale, sizeof(float), 1, _File);
}
