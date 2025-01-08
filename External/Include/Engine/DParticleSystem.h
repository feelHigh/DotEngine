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
    virtual void SaveToFile(FILE* _File) override;   // ���Ͽ� ������Ʈ�� ������ ����
    virtual void LoadFromFile(FILE* _File) override; // ���Ͽ� ������Ʈ�� ������ �ε�

private:
    Ptr<DParticleTickCS>    m_TickCS;
    DStructuredBuffer* m_ParticleBuffer;       // ��� ��ƼŬ ����
    DStructuredBuffer* m_SpawnCountBuffer;     // ��ƼŬ Ȱ��ȭ ���� ���޿� ����
    DStructuredBuffer* m_ModuleBuffer;         // Module Data Buffer

    Ptr<DTexture>           m_ParticleTex;
    float                   m_Time;                 // �����ð�    
    float                   m_BurstTime;            // SpawnBurst üũ�� ����

    int                     m_MaxParticeCount;      // ��ƼŬ �ִ� ����

    tParticleModule         m_Module;               // ��ƼŬ�� ��� ����

};

#endif // !ENGINE_DPARTICLE_SYSTEM_H