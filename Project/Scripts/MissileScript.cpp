#include "pch.h"
#include "MissileScript.h"

MissileScript::MissileScript()
	: DScript(UINT(SCRIPT_TYPE::MISSILESCRIPT))
	, m_Speed(1000.f)
{
}

MissileScript::~MissileScript()
{
}

void MissileScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();

	Ptr<DMaterial> pMtrl = GetRenderComponent()->GetMaterial();

	if (nullptr != pMtrl)
	{
		Ptr<DTexture> pTexture = DAssetMgr::GetInst()->Load<DTexture>(L"Texture\\Idle_Left.bmp", L"Texture\\Idle_Left.bmp");
		pMtrl->SetTexParam(TEX_0, pTexture);
	}
}

void MissileScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	vPos.y += DT * m_Speed;

	Transform()->SetRelativePos(vPos);
}

void MissileScript::BeginOverlap(DCollider2D* _OwnCollider
	, DGameObject* _OtherObject, DCollider2D* _OtherCollider)
{
	DeleteObject(_OtherObject);
}

void MissileScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, 4, 1, _File);
}

void MissileScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, 4, 1, _File);
}
