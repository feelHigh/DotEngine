#include "pch.h"
#include "DEditorMgr.h"

// Base Headers
#include "ComponentList.h"

// Engine Headers
#include "DEngine.h"

// Device Headers
#include "DDevice.h"

// Manager Headers
#include "DKeyMgr.h"
#include "DRenderMgr.h"
#include "DPathMgr.h"

// GameObject Headers
#include "DGameObjectEx.h"

// Editor Headers
#include "DEditor.h"

// Module Headers
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"
#include "ImGui\imgui_impl_win32.h"

// Scripts
#include <Scripts\EditorCameraScript.h>

DEditorMgr::DEditorMgr()
    : m_hNotifyHandle(nullptr)
{

}

DEditorMgr::~DEditorMgr()
{
    Delete_Vec(m_vecEditorObject);
    Delete_Map(m_mapUI);

    // ImGui Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}


void DEditorMgr::Init()
{
    CreateEditorObject();

    InitImGui();


    // Create a kernel object to monitor the Content folder
    wstring ContentPath = DPathMgr::GetInst()->GetContentPath();
    m_hNotifyHandle = FindFirstChangeNotification(ContentPath.c_str(), true
                                                , FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
                                                | FILE_ACTION_ADDED | FILE_ACTION_REMOVED);
}

void DEditorMgr::Tick()
{
    ShortCut();

    EditorObjectProgress();

    ImGuiProgress();

    ObserveContent();
}


void DEditorMgr::ShortCut()
{
    if (KEY_TAP(KEY::I))
    {
        DEditor* pUI = FindEditor("Inspector");

        if (pUI->IsActive())
            pUI->SetActive(false);
        else
            pUI->SetActive(true);
    }
}


void DEditorMgr::EditorObjectProgress()
{
    for (size_t i = 0; i < m_vecEditorObject.size(); ++i)
    {
        m_vecEditorObject[i]->Tick();
    }

    for (size_t i = 0; i < m_vecEditorObject.size(); ++i)
    {
        m_vecEditorObject[i]->FinalTick();
    }
}

void DEditorMgr::ImGuiTick()
{
    ImGui::ShowDemoWindow();

    for (const auto& pair : m_mapUI)
    {
        pair.second->Tick();
    }
}

void DEditorMgr::CreateEditorObject()
{
    DGameObject* pEditorCamera = new DGameObjectEx;
    pEditorCamera->SetName(L"EditorCamera");
    pEditorCamera->AddComponent(new DTransform);
    pEditorCamera->AddComponent(new DCamera);
    pEditorCamera->AddComponent(new EditorCameraScript);

    pEditorCamera->Camera()->SetLayerAll();
    pEditorCamera->Camera()->SetLayer(31, false);
    pEditorCamera->Camera()->SetFar(100000.f);
    pEditorCamera->Camera()->SetProjType(ORTHOGRAPHIC);

    m_vecEditorObject.push_back(pEditorCamera);

    DRenderMgr::GetInst()->RegisterEditorCamera(pEditorCamera->Camera());
}
