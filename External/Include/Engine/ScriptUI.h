#pragma once
#include "ComponentUI.h"

class DScript;

class ScriptUI :
    public ComponentUI
{
public:
    ScriptUI();
    ~ScriptUI();

public:
    virtual void Update() override;

public:
    void SetTargetScript(DScript* _Script);
    DScript* GetTargetScript() { return m_Script; }

private:
    void SelectPrefab(DWORD_PTR _ListUI);

private:
    class DScript*  m_Script;

    UINT            m_UIHeight;

    Ptr<DPrefab>*   m_SelectedPrefab;

};
