#include "pch.h"
#include "MenuUI.h"

// Base Headers
#include "AssetList.h"

// Manager Headers
#include "DLevelMgr.h"
#include "DAssetMgr.h"
#include "DEditorMgr.h"

// Level Headers
#include "DLevel.h"

// GameObject Headers
#include "DGameObject.h"

// Editor Headers
#include "Inspector.h"

// Module Headers
#include "DLevelSaveLoad.h"

// Component Headers
#include "DScript.h"

// Scripts
#include <Scripts\DScriptMgr.h>

MenuUI::MenuUI()
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Tick()
{
	if (!IsActive())
		return;

	if (ImGui::BeginMainMenuBar())
	{

		Update();

		ImGui::EndMainMenuBar();
	}
}

void MenuUI::Update()
{
	File();

	Level();

	GameObject();

	Assets();

	Editor();
}

void MenuUI::File()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Level Save"))
		{

		}

		if (ImGui::MenuItem("Level Load"))
		{

		}

		ImGui::EndMenu();
	}
}

void MenuUI::Level()
{
	DLevel* pCurLevel = DLevelMgr::GetInst()->GetCurrentLevel();

	ImGui::BeginDisabled(!pCurLevel);

	if (ImGui::BeginMenu("Level"))
	{
		LEVEL_STATE State = LEVEL_STATE::STOP;
		if (nullptr != pCurLevel)
			State = pCurLevel->GetState();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY == State);
		if (ImGui::MenuItem("Play"))
		{
			if (LEVEL_STATE::STOP == State)
			{
				wstring strLevelPath = DPathMgr::GetInst()->GetContentPath();
				strLevelPath += L"Level\\Temp.lv";
				DLevelSaveLoad::SaveLevel(strLevelPath, pCurLevel);
			}

			ChangeLevelState(LEVEL_STATE::PLAY);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY != State);
		if (ImGui::MenuItem("Pause"))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::STOP == State);
		if (ImGui::MenuItem("Stop"))
		{
			wstring StrLevelLoadPath = DPathMgr::GetInst()->GetContentPath();
			StrLevelLoadPath += L"Level\\Temp.lv";
			DLevel* pLoadedLevel = DLevelSaveLoad::LoadLevel(StrLevelLoadPath);
			ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

			// Inspector Clear 하기 (이전 오브젝트 정보를 보여주고 있을 수가 있기 때문에)				
			Inspector* pInspector = (Inspector*)DEditorMgr::GetInst()->FindEditor("Inspector");
			pInspector->SetTargetObject(nullptr);
			pInspector->SetTargetAsset(nullptr);
		}
		ImGui::EndDisabled();

		ImGui::EndMenu();
	}

	ImGui::EndDisabled();
}

void MenuUI::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object"))
		{

		}

		if (ImGui::BeginMenu("Add Component"))
		{
			ImGui::MenuItem("MeshRender");
			ImGui::MenuItem("Collider2D");
			ImGui::MenuItem("Camera");

			ImGui::EndMenu();
		}

		AddScript();


		ImGui::EndMenu();
	}
}

void MenuUI::AddScript()
{
	if (ImGui::BeginMenu("Add Script"))
	{
		vector<wstring> vecScriptsName;
		DScriptMgr::GetScriptInfo(vecScriptsName);

		for (size_t i = 0; i < vecScriptsName.size(); ++i)
		{
			if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
			{
				//DScript* pScript = DScriptMgr::GetScript(vecScriptsName[i]);

				// 인스펙터
				Inspector* pInspector = (Inspector*)DEditorMgr::GetInst()->FindEditor("Inspector");

				// 타겟 오브젝트 알아냄
				DGameObject* pObject = pInspector->GetTargetObject();
				if (nullptr != pObject)
				{
					// 오브젝트에, 선택한 스크립트를 추가해줌
					DScript* pScript = DScriptMgr::GetScript(vecScriptsName[i]);
					pObject->AddComponent(pScript);
				}
			}
		}

		ImGui::EndMenu();
	}

}

void MenuUI::Assets()
{
	if (ImGui::BeginMenu("Assets"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			Ptr<DMaterial> pMtrl = new DMaterial;
			wstring Key = GetAssetKey(ASSET_TYPE::MATERIAL, L"Default Material");
			DAssetMgr::GetInst()->AddAsset<DMaterial>(Key, pMtrl);
			pMtrl->Save(Key);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Editor()
{
	if (ImGui::BeginMenu("Editor"))
	{
		DEditor* pSpriteEditor = DEditorMgr::GetInst()->FindEditor("SpriteEditor");
		bool IsActive = pSpriteEditor->IsActive();

		if (ImGui::MenuItem("Sprite Editor", nullptr, &IsActive))
		{
			DEditorMgr::GetInst()->FindEditor("SpriteEditor")->SetActive(IsActive);
		}

		ImGui::EndMenu();
	}
}

wstring MenuUI::GetAssetKey(ASSET_TYPE _Type, const wstring& _KeyFormat)
{
	wstring Key;

	switch (_Type)
	{
	case ASSET_TYPE::MATERIAL:
	{
		Key = wstring(L"Material\\") + _KeyFormat + L" %d.mtrl";
	}
	break;
	case ASSET_TYPE::SPRITE:
	{
		Key = wstring(L"Sprite\\") + _KeyFormat + L" %d.sprite";
	}
	break;
	case ASSET_TYPE::FLIPBOOK:
	{
		Key = wstring(L"Flipbook\\") + _KeyFormat + L" %d.flip";
	}
	break;
	case ASSET_TYPE::PREFAB:
	{
		Key = wstring(L"Prefab\\") + _KeyFormat + L" %d.pref";
	}
	break;
	default:
		assert(nullptr);
		break;
	}

	wchar_t szKey[255] = {};
	wstring FilePath = DPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < 0xffffffff; ++i)
	{
		swprintf_s(szKey, 255, Key.c_str(), i);

		if (false == std::filesystem::exists(FilePath + szKey))
		{
			break;
		}
	}

	return szKey;
}
