#include "pch.h"
#include "DRenderComponent.h"

// Manager Headers
#include "DAssetMgr.h"
#include "DLevelMgr.h"

// Level Headers
#include "DLevel.h"

DRenderComponent::DRenderComponent(COMPONENT_TYPE _Type)
	: DComponent(_Type)
	, m_Mesh(nullptr)
	, m_Mtrl(nullptr)
{
}

DRenderComponent::DRenderComponent(const DRenderComponent& _Origin)
	: DComponent(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_Mtrl(_Origin.m_Mtrl)
	, m_SharedMtrl(_Origin.m_Mtrl)
{
	DLevel* pCurLevel = DLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr != pCurLevel)
	{
		assert(!(pCurLevel->GetState() != LEVEL_STATE::PLAY
			&& nullptr != _Origin.m_DynamicMtrl));
	}

	if (nullptr != _Origin.m_DynamicMtrl)
	{
		GetDynamicMaterial();
	}
}

DRenderComponent::~DRenderComponent()
{
}

void DRenderComponent::SetMaterial(Ptr<DMaterial> _Mtrl)
{
	m_Mtrl = m_SharedMtrl = _Mtrl;
}

Ptr<DMaterial> DRenderComponent::GetSharedMtrl()
{
	m_Mtrl = m_SharedMtrl;
	return m_Mtrl;
}

Ptr<DMaterial> DRenderComponent::GetDynamicMaterial()
{
	// Dynamic material generation and return should only be called if the game (level) is in Play mode.
	assert(DLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY);

	if (nullptr != m_DynamicMtrl)
		return m_Mtrl = m_DynamicMtrl;

	m_Mtrl = m_DynamicMtrl = m_SharedMtrl->Clone();

	return m_Mtrl;
}

void DRenderComponent::SaveDataToFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_Mtrl, _File);
	SaveAssetRef(m_SharedMtrl, _File);
}

void DRenderComponent::LoadDataFromFile(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	LoadAssetRef(m_Mtrl, _File);
	LoadAssetRef(m_SharedMtrl, _File);
}
