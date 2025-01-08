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

    // �������� ���� �� ��ȯ
    Ptr<DMaterial> GetDynamicMaterial();

protected:
    void SaveDataToFile(FILE* _File);
    void LoadDataFromFile(FILE* _File);

private:
    Ptr<DMesh>          m_Mesh;

    Ptr<DMaterial>      m_Mtrl;         // ���� ������� ����
    Ptr<DMaterial>      m_SharedMtrl;   // ���� ����(������)
    Ptr<DMaterial>      m_DynamicMtrl;  // �ӽ� ����

};

#endif // !ENGINE_DRENDER_COMPONENT_H