#include "pch.h"
#include "DParticleTickCS.h"

// Device Headers
#include "DStructuredBuffer.h"

// Manager Headers
#include "DAssetMgr.h"

DParticleTickCS::DParticleTickCS()
	: DComputeShader(1024, 1, 1, L"Shader\\ParticleTickCS.hlsl", "CS_ParticleTick")
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
{
	m_NoiseTex = DAssetMgr::GetInst()->FindAsset<DTexture>(L"Texture\\Noise\\noise_03.jpg");
}

DParticleTickCS::~DParticleTickCS()
{
}

int DParticleTickCS::Binding()
{
	if (nullptr == m_ParticleBuffer || nullptr == m_SpawnCountBuffer || nullptr == m_NoiseTex)
		return E_FAIL;

	m_ParticleBuffer->Binding_CS_UAV(0);
	m_SpawnCountBuffer->Binding_CS_UAV(1);

	m_NoiseTex->Binding_CS_SRV(20);
	m_ModuleBuffer->Binding_CS_SRV(21);

	m_Const.iArr[0] = m_ParticleBuffer->GetElementCount();
	m_Const.v4Arr[0] = m_ParticleWorldPos;

	return S_OK;
}

void DParticleTickCS::CalcGroupNum()
{
	m_GroupX = m_ParticleBuffer->GetElementCount() / m_ThreadPerGroupX;

	if (m_ParticleBuffer->GetElementCount() % m_ThreadPerGroupX)
		m_GroupX += 1;

	m_GroupY = 1;
	m_GroupZ = 1;
}

void DParticleTickCS::Clear()
{
	m_ParticleBuffer->Clear_CS_UAV();
	m_ParticleBuffer = nullptr;

	m_NoiseTex->Clear_CS_SRV();
	m_ModuleBuffer->Clear_CS_SRV();
	m_ModuleBuffer = nullptr;
}
