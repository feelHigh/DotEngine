#include "pch.h"
#include "TileMapEditor.h"

#include "DEditorMgr.h"
#include "TE_TileSetView.h"
#include "TE_TileMapView.h"
#include "TE_TileMapMod.h"

TileMapEditor::TileMapEditor()
    : m_TileSetView(nullptr)
    , m_TileMapView(nullptr)
    , m_TileMapMod(nullptr)
{
    UseMenuBar(true);
}

TileMapEditor::~TileMapEditor()
{
}

void TileMapEditor::Init()
{
    // Initialize tile set view
    m_TileSetView = (TE_TileSetView*)DEditorMgr::GetInst()->FindEditor("TE_TileSetView");
    m_TileSetView->m_Owner = this;

    // Initialize tile map view
    m_TileMapView = (TE_TileMapView*)DEditorMgr::GetInst()->FindEditor("TE_TileMapView");
    m_TileMapView->m_Owner = this;

    // Initialize tile map modifier
    m_TileMapMod = (TE_TileMapMod*)DEditorMgr::GetInst()->FindEditor("TE_TileMapMod");
    m_TileMapMod->m_Owner = this;

    // For testing
    m_TileSetView->SetTilesetTexture(DAssetMgr::GetInst()->Load<DTexture>(L"Texture\\NewForest\\Tile_NewForestFloor.png", L"Texture\\NewForest\\Tile_NewForestFloor.png"));
}

void TileMapEditor::Update()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Tilemap"))
            {
                // Initialize a new tilemap
                m_TileMapView->ResetTileMap();
            }

            if (ImGui::MenuItem("Save Tilemap"))
            {
                // Delegate to TileMapMod to save
                //m_TileMapMod->SaveTileMap();
            }

            if (ImGui::MenuItem("Load Tilemap"))
            {
                // Delegate to TileMapMod to load
                //m_TileMapMod->LoadTileMap();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
            {
                // Handle exit logic
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Clear Tilemap"))
            {
                m_TileMapView->ClearTileMap();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            // Add view-related menu items here
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void TileMapEditor::Activate()
{
    m_TileSetView->SetActive(true);
    m_TileMapView->SetActive(true);
    m_TileMapMod->SetActive(true);
}

void TileMapEditor::Deactivate()
{
    m_TileSetView->SetActive(false);
    m_TileMapView->SetActive(false);
    m_TileMapMod->SetActive(false);
}
