#pragma once
#include "ComponentUI.h"

class CameraUI :
    public ComponentUI
{
public:
    CameraUI();
    ~CameraUI();

private:
    virtual void Update() override;

private:
    void LayerCheck();
    void Projection();

private:
    bool    m_ShowLayerCheck;

};
