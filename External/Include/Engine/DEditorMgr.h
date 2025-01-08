#pragma once
#include "Singleton.h"

class DGameObject;
class DEditor;

class DEditorMgr :
    public DSingleton<DEditorMgr>
{
    SINGLE(DEditorMgr);
public:
    void Init();
    void Tick();

private:
    void ShortCut();

    void CreateEditorObject();
    void EditorObjectProgress();

    void InitImGui();
    void ObserveContent();
    void ImGuiProgress();
    void ImGuiTick();
    void CreateEditor();

public:
    DEditor* FindEditor(const string& Name);

private:
    vector<DGameObject*>        m_vecEditorObject;
    map<string, DEditor*>       m_mapUI;

    HANDLE                      m_hNotifyHandle;

};
