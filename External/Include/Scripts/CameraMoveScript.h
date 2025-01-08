#pragma once

#include <Engine\DScript.h>

class CameraMoveScript :
    public DScript
{
public:
    CLONE(CameraMoveScript);
    CameraMoveScript();
    ~CameraMoveScript();

public:
    virtual void Tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void OrthoGraphicMove();
    void PerspectiveMove();

private:
    float   m_CamSpeed;

};
