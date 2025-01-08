#include "pch.h"
#include "DSetColorCS.h"

DSetColorCS::DSetColorCS()
	: DComputeShader(32, 32, 1, L"Shader\\SetColorCS.hlsl", "CS_SetColor")
{
}

DSetColorCS::~DSetColorCS()
{
}

int DSetColorCS::Binding()
{
	if (nullptr == m_TargetTex || nullptr == m_TargetTex->GetUAV())
		return E_FAIL;

	m_TargetTex->Binding_CS_UAV(0);
	m_Const.v4Arr[0] = m_ClearColor;

	m_Const.iArr[0] = m_TargetTex->GetWidth();
	m_Const.iArr[1] = m_TargetTex->GetHeight();

	return S_OK;
}

void DSetColorCS::CalcGroupNum()
{
	m_GroupX = m_TargetTex->GetWidth() / m_ThreadPerGroupX;
	m_GroupY = m_TargetTex->GetHeight() / m_ThreadPerGroupY;
	m_GroupZ = 1;


	if (m_TargetTex->GetWidth() % m_ThreadPerGroupX)
		m_GroupX += 1;
	if (m_TargetTex->GetHeight() % m_ThreadPerGroupY)
		m_GroupY += 1;
}

void DSetColorCS::Clear()
{
	m_TargetTex->Clear_CS_UAV();
	m_TargetTex = nullptr;
}
