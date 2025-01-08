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
    virtual void SaveToFile(FILE* _File) override;   // 파일에 컴포넌트의 정보를 저장
    virtual void LoadFromFile(FILE* _File) override; // 파일에 컴포넌트의 정보를 로드

private:
    Ptr<DParticleTickCS>    m_TickCS;
    DStructuredBuffer* m_ParticleBuffer;       // 모든 파티클 정보
    DStructuredBuffer* m_SpawnCountBuffer;     // 파티클 활성화 숫자 전달용 버퍼
    DStructuredBuffer* m_ModuleBuffer;         // Module Data Buffer

    Ptr<DTexture>           m_ParticleTex;
    float                   m_Time;                 // 누적시간    
    float                   m_BurstTime;            // SpawnBurst 체크용 변수

    int                     m_MaxParticeCount;      // 파티클 최대 개수

    tParticleModule         m_Module;               // 파티클의 기능 정의

};

#endif // !ENGINE_DPARTICLE_SYSTEM_H