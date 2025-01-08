#pragma once
#include "DComputeShader.h"

class DStructuredBuffer;

class DParticleTickCS :
    public DComputeShader
{
public:
    DParticleTickCS();
    ~DParticleTickCS();

public:
    virtual int Binding() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

public:
    void SetParticleBuffer(DStructuredBuffer* _Buffer) { m_ParticleBuffer = _Buffer; }
    void SetSpawnCount(DStructuredBuffer* _Buffer) { m_SpawnCountBuffer = _Buffer; }
    void SetModuleBuffer(DStructuredBuffer* _ModuleBuffer) { m_ModuleBuffer = _ModuleBuffer; }

    void SetParticleWorldPos(Vec3 _Pos) { m_ParticleWorldPos = _Pos; }
    void SetNoiseTexture(Ptr<DTexture> _Noise) { m_NoiseTex = _Noise; }

private:
    DStructuredBuffer*  m_ParticleBuffer;
    DStructuredBuffer*  m_SpawnCountBuffer;
    DStructuredBuffer*  m_ModuleBuffer;

    Ptr<DTexture>       m_NoiseTex;
    Vec3                m_ParticleWorldPos;

};
