#pragma once
#include "TE_Sub.h"
#include "DTileMap.h"

class TE_TileMapMod
    : public TE_Sub
{
public:
    TE_TileMapMod();
    ~TE_TileMapMod();

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    // Get/set grid dimensions
    int GetNumRows() { return m_NumRows; }
    int GetNumColumns() { return m_NumColumns; }
    void SetGridSize(int _rows, int _columns);

public:
    // Renders the current tilemap to a texture
    Ptr<DTexture> RenderTileMapToTexture();

    // Helper function to render a single tile
    void RenderSpriteTile(ID3D11DeviceContext* pContext, Ptr<DTexture> texture,
        float x, float y, float width, float height,
        float u0, float v0, float u1, float v1);

public:
    // Save the current tilemap to file
    void SaveTileMap();

    void SaveTileMapMetadata(const wstring& _Filename);

    // Load a tilemap from file
    void LoadTileMap();

private:
    int m_NumRows;              // Number of rows in the tile map
    int m_NumColumns;           // Number of columns in the tile map

    char m_SaveFileName[256];   // Buffer for save file name input

};
