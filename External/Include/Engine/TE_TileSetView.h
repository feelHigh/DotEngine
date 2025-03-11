#pragma once
#include "TE_Sub.h"

class TE_TileSetView :
    public TE_Sub
{
public:
    TE_TileSetView();
    ~TE_TileSetView();

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    Ptr<DTexture> GetTilesetTexture() { return m_TilesetTexture; }
    void SetTilesetTexture(Ptr<DTexture> _texture) { m_TilesetTexture = _texture; }

    Vec2 GetSelectedTile() { return m_SelectedTile; }
    void SetSelectedTile(Vec2 _tile) { m_SelectedTile = _tile; }

    Vec2 GetTileSize() { return Vec2(m_TileWidth, m_TileHeight); }
    void SetTileSize(float _width, float _height) { m_TileWidth = _width; m_TileHeight = _height; }

    int GetRows() { return m_Rows; }
    int GetColumns() { return m_Columns; }
    void SetGridSize(int _rows, int _columns) { m_Rows = _rows; m_Columns = _columns; }

private:
    void DrawTilesetGrid();
    void HandleTileSelection();

private:
    Ptr<DTexture> m_TilesetTexture;
    Vec2 m_SelectedTile;      // Selected tile coordinates (column, row)

    float m_TileWidth;        // Width of a single tile
    float m_TileHeight;       // Height of a single tile
    int m_Rows;               // Number of rows in the tileset
    int m_Columns;            // Number of columns in the tileset

    float m_WheelScale;       // Zoom scale for the tileset view
    Vec2 m_CanvasScroll;      // Scroll position for panning
    bool m_RightClickDragging;// Flag for right-click dragging to pan
    Vec2 m_LastMousePos;      // Last mouse position for tracking drag
};
