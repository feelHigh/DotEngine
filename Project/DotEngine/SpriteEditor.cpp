#include "pch.h"
#include "SpriteEditor.h"

#include "DEditorMgr.h"
#include "SE_AtlasView.h"
#include "SE_SpriteView.h"
#include "SE_SpriteMod.h"

SpriteEditor::SpriteEditor()
	: m_AtlasView(nullptr)
	, m_SpriteView(nullptr)
	, m_SpriteMod(nullptr)
{
	UseMenuBar(true);
}

SpriteEditor::~SpriteEditor()
{
}

void SpriteEditor::Init()
{
	m_AtlasView = (SE_AtlasView*)DEditorMgr::GetInst()->FindEditor("SE_AtlasView");
	m_SpriteView = (SE_SpriteView*)DEditorMgr::GetInst()->FindEditor("SE_SpriteView");
	m_SpriteMod = (SE_SpriteMod*)DEditorMgr::GetInst()->FindEditor("SE_SpriteMod");

	m_AtlasView->SetAtlasTex(DAssetMgr::GetInst()->Load<DTexture>(L"Texture\\Dalia-Idle.png", L"Texture\\Dalia-Idle.png"));
}

void SpriteEditor::Update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load File", nullptr, true))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			bool b_AtlasView = m_AtlasView->IsActive();
			bool b_SpriteView = m_SpriteView->IsActive();
			bool b_SpriteMod = m_SpriteMod->IsActive();

			if (ImGui::MenuItem("Atlas View", nullptr, &b_AtlasView))
			{
				m_AtlasView->SetActive(b_AtlasView);
			}

			if (ImGui::MenuItem("Sprite View", nullptr, &b_SpriteView))
			{
				m_SpriteView->SetActive(b_SpriteView);
			}
			
			if (ImGui::MenuItem("Sprite Mod", nullptr, &b_SpriteMod))
			{
				m_SpriteMod->SetActive(b_SpriteMod);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void SpriteEditor::Activate()
{
	m_AtlasView->SetActive(true);
	m_SpriteView->SetActive(true);
	m_SpriteMod->SetActive(true);
}

void SpriteEditor::Deactivate()
{
	m_AtlasView->SetActive(false);
	m_SpriteView->SetActive(false);
	m_SpriteMod->SetActive(false);
}
