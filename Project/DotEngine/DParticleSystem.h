#ifndef ENGINE_DPARTICLE_SYSTEM_H
#define ENGINE_DPARTICLE_SYSTEM_H

#include "DRenderComponent.h"
#include "DParticleTickCS.h"

class DStructuredBuffer;

struct tSpawnCount
{
    UINT    SpawnCount;
    UINT    padding[3];
};

class DParticleSystem :
    public DRenderComponent
{
public:
    CLONE(DParticleSystem);
    DParticleSystem();
    DParticleSystem(const DParticleSystem& _Other);
    ~DParticleSystem();

public:
    virtual void FinalTick() override;
    virtual void Render() override;

public:
    void CaculateSpawnCount();

public:
    void SetParticleTexture(Ptr<DTexture> _Texture) { m_ParticleTex = _Texture; }

public:
    virtual void SaveToFile(FILE* _File) override;   // Save component information to a file
    virtual void LoadFromFile(FILE* _File) override; // Load component information into a file

private:
    Ptr<DParticleTickCS>    m_TickCS;
    DStructuredBuffer*      m_ParticleBuffer;       // All Particle Information
    DStructuredBuffer*      m_SpawnCountBuffer;     // Buffer for Particle Activation Numbers Transfer
    DStructuredBuffer*      m_ModuleBuffer;         // Module Data Buffer

    Ptr<DTexture>           m_ParticleTex;
    float                   m_Time;                 // Accumulative time
    float                   m_BurstTime;            // Variables for checking SpawnBurst

    int                     m_MaxParticeCount;      // Max particle count

    tParticleModule         m_Module;               // Define particle functionality

};

#endif // !ENGINE_DPARTICLE_SYSTEM_H