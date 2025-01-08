#pragma once
#include "DEditor.h"

class ComponentUI :
    public DEditor
{
public:
    ComponentUI(COMPONENT_TYPE _Type);
    ~ComponentUI();

public:
    void SetTargetObject(DGameObject* _Object);
    DGameObject* GetTargetObject() { return m_TargetObject; }

    COMPONENT_TYPE GetComponentUIType() { return m_Type; }

protected:
    void Title();

private:
    class DGameObject* m_TargetObject;
    const COMPONENT_TYPE    m_Type;

};
