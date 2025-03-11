#pragma once
#include "TE_Sub.h"

struct TileData
{
    Vec2 TileIndex;     // Index in the tileset (column, row)
    bool IsOccupied;    // Whether the tile has data
};

class TE_TileMapView
    : public TE_Sub
{
public:
    TE_TileMapView();
    ~TE_TileMapView();

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    void SetGridSize(int _rows, int _columns);
    void ResetTileMap();
    void ClearTileMap();

    void PlaceTile(int _row, int _col, Vec2 _tileIndex);
    void RemoveTile(int _row, int _col);
    TileData* GetTileData() { return m_Grid.data(); }
    int GetNumRows() { return m_NumRows; }
    int GetNumColumns() { return m_NumColumns; }

    // Get the width of a single tile
    float GetTileWidth() const { return m_TileWidth; }

    // Get the height of a single tile
    float GetTileHeight() const { return m_TileHeight; }

    // Set the size of tiles in the grid
    void SetTileSize(float _width, float _height);

private:
    void DrawTileMapGrid();
    void HandleTilePlacement();

private:
    vector<TileData> m_Grid;   // Tile data for the entire grid
    int m_NumRows;             // Number of rows in the tile map
    int m_NumColumns;          // Number of columns in the tile map

    float m_TileWidth;         // Width of a single tile
    float m_TileHeight;        // Height of a single tile

    float m_ZoomScale;         // Zoom scale for the tile map view
    Vec2 m_CanvasScroll;       // Scroll position for panning
    bool m_RightClickDragging; // Flag for right-click dragging to pan
    Vec2 m_LastMousePos;       // Last mouse position for tracking drag

    bool m_EraserMode;         // Flag for eraser mode

};
