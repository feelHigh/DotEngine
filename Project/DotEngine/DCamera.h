#ifndef ENGINE_DCAMERA_H
#define ENGINE_DCAMERA_H

#include "DComponent.h"

class DGameObject;

enum PROJ_TYPE
{
    ORTHOGRAPHIC,
    PERSPECTIVE,
};

class DCamera :
    public DComponent
{
public:
    CLONE(DCamera);
    DCamera();
    ~DCamera();

public:
    virtual void Begin() override;
    virtual void FinalTick() override;

public:
    void Render();

public:
    void SetPriority(int _Priority) { m_Priority = _Priority; }
    void SetLayer(UINT _LayerIdx, bool _bCheck)
    {
        if (_bCheck)
            m_LayerCheck |= (1 << _LayerIdx);
        else
            m_LayerCheck &= ~(1 << _LayerIdx);
    }
    void SetLayerAll() { m_LayerCheck = 0xffffffff; }

    bool GetLayerCheck(UINT _LayerIdx) { return m_LayerCheck & (1 << _LayerIdx); }


    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetWidth(float _Width)
    {
        m_Width = _Width;
        m_AspectRatio = m_Width / m_Height;
    }

    void SetHeight(float _Height)
    {
        m_Height = _Height;
        m_AspectRatio = m_Width / m_Height;
    }

    float GetWidth() { return m_Width; }
    float GetHeight() { return m_Height; }
    float GetAspectRatio() { return m_AspectRatio; }

    void SetFar(float _Far) { m_Far = _Far; }
    float GetFar() { return m_Far; }

    void SetFOV(float _FOV) { m_FOV = _FOV; }
    float GetFOV() { return m_FOV; }

    void SetScale(float _Scale) { m_ProjectionScale = _Scale; }
    float GetScale() { return m_ProjectionScale; }

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void SortGameObject();
    void Render_Effect();

private:
    int                     m_Priority;
    UINT                    m_LayerCheck;       // Set only the desired layer to be captured on camera

    PROJ_TYPE               m_ProjType;

    float                   m_Width;
    float                   m_Height;
    float                   m_AspectRatio;
    float                   m_Far;              // Distance of view that the camera can see
    float                   m_FOV;              // Field Of View
    float                   m_ProjectionScale;

    Matrix                  m_matView;
    Matrix                  m_matProj;

    vector<DGameObject*>    m_vecOpaque;
    vector<DGameObject*>    m_vecMasked;
    vector<DGameObject*>    m_vecTransparent;
    vector<DGameObject*>    m_vecEffect;
    vector<DGameObject*>    m_vecParticles;
    vector<DGameObject*>    m_vecPostProcess;
    vector<DGameObject*>    m_vecUI;

};

#endif // !ENGINE_DCAMERA_H