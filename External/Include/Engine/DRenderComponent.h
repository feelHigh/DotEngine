#ifndef ENGINE_DRENDER_COMPONENT_H
#define ENGINE_DRENDER_COMPONENT_H

#include "DComponent.h"

class DRenderComponent :
    public DComponent
{
public:
    CLONE_DISABLE(DRenderComponent);
    DRenderComponent(COMPONENT_TYPE _Type);
    DRenderComponent(const DRenderComponent& _Origin);
    ~DRenderComponent();

public:
    virtual void FinalTick() = 0;
    virtual void Render() = 0;

public:
    void SetMesh(Ptr<DMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(Ptr<DMaterial> _Mtrl);

    Ptr<DMesh> GetMesh() { return m_Mesh; }
    Ptr<DMaterial> GetMaterial() { return m_Mtrl; }
    Ptr<DMaterial> GetSharedMtrl();

    // 동적재질 생성 및 반환
    Ptr<DMaterial> GetDynamicMaterial();

protected:
    void SaveDataToFile(FILE* _File);
    void LoadDataFromFile(FILE* _File);

private:
    Ptr<DMesh>          m_Mesh;

    Ptr<DMaterial>      m_Mtrl;         // 현재 사용중인 재질
    Ptr<DMaterial>      m_SharedMtrl;   // 공유 재질(마스터)
    Ptr<DMaterial>      m_DynamicMtrl;  // 임시 재질

};

#endif // !ENGINE_DRENDER_COMPONENT_H