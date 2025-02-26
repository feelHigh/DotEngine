#include "pch.h"
#include "TileMapEditor.h"

#include "DEditorMgr.h"
#include "TE_TileMapMod.h"
#include "TE_TileMapView.h"
#include "TE_TileSetView.h"

TileMapEditor::TileMapEditor()
	: m_TileMapMod(nullptr)
	, m_TileMapView(nullptr)
	, m_TileSetView(nullptr)
{
	UseMenuBar(true);
}

TileMapEditor::~TileMapEditor()
{
}

void TileMapEditor::Init()
{
	m_TileMapMod = (TE_TileMapMod*)DEditorMgr::GetInst()->FindEditor("TE_TileMapMod");
	m_TileMapView = (TE_TileMapView*)DEditorMgr::GetInst()->FindEditor("TE_TileMapView");
	m_TileSetView = (TE_TileSetView*)DEditorMgr::GetInst()->FindEditor("TE_TileSetView");

	m_TileMapMod->m_Owner = this;
	m_TileMapView->m_Owner = this;
	m_TileSetView->m_Owner = this;

	m_TileSetView->SetTileSetTex(DAssetMgr::GetInst()->Load<DTexture>(L"Texture\\NewForest\\Tile_NewForestFloor.png"
																	, L"Texture\\NewForest\\Tile_NewForestFloor.png"));
}

void TileMapEditor::Update()
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
			bool b_TileMapView = m_TileMapView->IsActive();
			bool b_TileMapMod = m_TileMapMod->IsActive();
			bool b_TileSetView = m_TileSetView->IsActive();

			if (ImGui::MenuItem("TileMap View", nullptr, &b_TileMapView))
			{
				m_TileMapView->SetActive(b_TileMapView);
			}

			if (ImGui::MenuItem("TileMap Mod", nullptr, &b_TileMapMod))
			{
				m_TileMapMod->SetActive(b_TileMapMod);
			}

			if (ImGui::MenuItem("TileSet View", nullptr, &b_TileSetView))
			{
				m_TileSetView->SetActive(b_TileSetView);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void TileMapEditor::Activate()
{
	m_TileMapView->SetActive(true);
	m_TileMapMod->SetActive(true);
	m_TileSetView->SetActive(true);
}

void TileMapEditor::Deactivate()
{
	m_TileMapView->SetActive(false);
	m_TileMapMod->SetActive(false);
	m_TileSetView->SetActive(false);
}
