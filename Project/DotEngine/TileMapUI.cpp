#include "pch.h"
#include "TileMapUI.h"

// GameObject Headers
#include "DGameObject.h"

// Component Headers
#include "DTileMap.h"

TileMapUI::TileMapUI()
	: ComponentUI(COMPONENT_TYPE::TILEMAP)
{
}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::Update()
{
	Title();

	DTileMap* pTileMap = GetTargetObject()->TileMap();

	Vec2 vRowCol = pTileMap->GetRowCol();
	Vec2 vTileSize = pTileMap->GetTileSize();
	Vec2 vAtlasTileSize = pTileMap->GetAtlasTileSize();

	ImGui::Text("RowCol");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##Pos", vRowCol);

	ImGui::Text("TileSize");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##Scale", vTileSize);

	ImGui::Text("AtlasTileSize");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##Rot", vAtlasTileSize);

	pTileMap->SetRowCol(vRowCol.x, vRowCol.y);
	pTileMap->SetTileSize(vTileSize);
	pTileMap->SetAtlasTileSize(vAtlasTileSize);

}
