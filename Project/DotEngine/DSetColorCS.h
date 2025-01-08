#pragma once
#include "DComputeShader.h"

class DSetColorCS :
    public DComputeShader
{
public:
    DSetColorCS();
    ~DSetColorCS();

public:
    virtual int Binding() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

public:
    void SetTargetTexture(Ptr<DTexture> _Tex) { m_TargetTex = _Tex; }
    void SetClearColor(Vec4 _Color) { m_ClearColor = _Color; }

private:
    Ptr<DTexture>       m_TargetTex;
    Vec4                m_ClearColor;

};
