#include "pch.h"
#include "DLight2D.h"

// Manager Headers
#include "DRenderMgr.h"

// Component Headers
#include "DTransform.h"

DLight2D::DLight2D()
	: DComponent(COMPONENT_TYPE::LIGHT2D)
	, m_Info{}
{
}

DLight2D::~DLight2D()
{
}

void DLight2D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR::RIGHT);

	//Register to RenderMgr
	DRenderMgr::GetInst()->RegisterLight2D(this);
}

void DLight2D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.Type = _Type;
}

void DLight2D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void DLight2D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}
