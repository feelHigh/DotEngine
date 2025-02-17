#include "pch.h"
#include "TE_TileMapMod.h"
#include "TE_TileMapView.h"

TE_TileMapMod::TE_TileMapMod()
    : m_NumRows(9)
    , m_NumColumns(9)
{
}

TE_TileMapMod::~TE_TileMapMod()
{
}

void TE_TileMapMod::Init()
{
}

void TE_TileMapMod::Update()
{
    ImGui::Text("Tile Map Modifier");
    ImGui::InputInt("Rows", &m_NumRows);
    ImGui::InputInt("Columns", &m_NumColumns);

    m_NumRows = max(m_NumRows, 1);
    m_NumColumns = max(m_NumColumns, 1);

    GetTileMapView()->SetGridSize(m_NumRows, m_NumColumns);
}
