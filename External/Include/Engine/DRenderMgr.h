#pragma once
#include "Singleton.h"

class DCamera;
class DGameObject;
class DLight2D;
class DStructuredBuffer;

class DRenderMgr :
    public DSingleton<DRenderMgr>
{
    SINGLE(DRenderMgr);
public:
    void Init();
    void Tick();

private:
    void RenderStart();
    void Clear();
    void RenderDebugShape();

public:
    void RegisterCamera(DCamera* _Cam, int _CamPriority);
    void RegisterEditorCamera(DCamera* _Cam) { m_EditorCamera = _Cam; }
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.push_back(_Info); }
    void RegisterLight2D(DLight2D* _Light) { m_vecLight2D.push_back(_Light); }
    void PostProcessCopy();

private:
    vector<DCamera*>        m_vecCam;
    DCamera*                m_EditorCamera;

    DGameObject*            m_DebugObject;
    list<tDebugShapeInfo>   m_DebugShapeList;

    // Light
    vector<DLight2D*>       m_vecLight2D;
    DStructuredBuffer*      m_Light2DBuffer;

    // PostProcess
    Ptr<DTexture>           m_PostProcessTex;

    // DownScale
    Ptr<DTexture>           m_DownScaleTex;

};
