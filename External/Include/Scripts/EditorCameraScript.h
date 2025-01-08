#pragma once
#include <Engine\DScript.h>

class EditorCameraScript :
    public DScript
{
public:
    CLONE(EditorCameraScript);
    EditorCameraScript();
    ~EditorCameraScript();

public:
    virtual void Tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File)override;

private:
    void OrthoGraphicMove();
    void PerspectiveMove();

private:
    float   m_Speed;

};
