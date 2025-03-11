#include "pch.h"
#include "TE_TileMapView.h"
#include "TE_TileSetView.h"

TE_TileMapView::TE_TileMapView()
    : m_NumRows(10)
    , m_NumColumns(10)
    , m_TileWidth(32.f)
    , m_TileHeight(32.f)
    , m_ZoomScale(1.f)
    , m_CanvasScroll(Vec2(0.f, 0.f))
    , m_RightClickDragging(false)
    , m_LastMousePos(Vec2(0.f, 0.f))
    , m_EraserMode(false)
{
    ResetTileMap();
}

TE_TileMapView::~TE_TileMapView()
{
}

void TE_TileMapView::Init()
{
    // Initialize with default grid
    SetGridSize(10, 10);
}

void TE_TileMapView::Update()
{
    // Controls for tile map view
    ImGui::Checkbox("Eraser Mode", &m_EraserMode);
    // Display tile map grid and handle user interaction
    DrawTileMapGrid();
    HandleTilePlacement();
}

void TE_TileMapView::SetGridSize(int _rows, int _columns)
{
    // Validate input
    _rows = max(1, _rows);
    _columns = max(1, _columns);
    // If dimensions change, resize the grid
    if (_rows != m_NumRows || _columns != m_NumColumns)
    {
        m_NumRows = _rows;
        m_NumColumns = _columns;
        // Create a new grid with the new dimensions
        vector<TileData> newGrid(_rows * _columns);
        // Copy data from the old grid if possible
        for (int row = 0; row < min(m_NumRows, (int)m_Grid.size() / m_NumColumns); ++row)
        {
            for (int col = 0; col < min(m_NumColumns, m_NumColumns); ++col)
            {
                int oldIndex = row * m_NumColumns + col;
                int newIndex = row * _columns + col;
                if (oldIndex < (int)m_Grid.size() && newIndex < (int)newGrid.size())
                {
                    newGrid[newIndex] = m_Grid[oldIndex];
                }
            }
        }
        // Replace the old grid with the new one
        m_Grid = newGrid;
    }
}

void TE_TileMapView::ResetTileMap()
{
    // Reset to default size
    m_NumRows = 10;
    m_NumColumns = 10;
    // Clear the grid
    m_Grid.resize(m_NumRows * m_NumColumns);
    for (auto& tile : m_Grid)
    {
        tile.IsOccupied = false;
        tile.TileIndex = Vec2(-1.f, -1.f);
    }
}

void TE_TileMapView::ClearTileMap()
{
    // Clear all tiles but keep the grid size
    for (auto& tile : m_Grid)
    {
        tile.IsOccupied = false;
        tile.TileIndex = Vec2(-1.f, -1.f);
    }
}

void TE_TileMapView::PlaceTile(int _row, int _col, Vec2 _tileIndex)
{
    // Validate coordinates
    if (_row < 0 || _row >= m_NumRows || _col < 0 || _col >= m_NumColumns)
        return;
    // Place the tile
    int index = _row * m_NumColumns + _col;
    m_Grid[index].IsOccupied = true;
    m_Grid[index].TileIndex = _tileIndex;
}

void TE_TileMapView::RemoveTile(int _row, int _col)
{
    // Validate coordinates
    if (_row < 0 || _row >= m_NumRows || _col < 0 || _col >= m_NumColumns)
        return;
    // Remove the tile
    int index = _row * m_NumColumns + _col;
    m_Grid[index].IsOccupied = false;
    m_Grid[index].TileIndex = Vec2(-1.f, -1.f);
}

void TE_TileMapView::SetTileSize(float _width, float _height)
{
    // Validate input (prevent zero or negative sizes)
    _width = max(1.0f, _width);
    _height = max(1.0f, _height);

    // Update tile dimensions
    m_TileWidth = _width;
    m_TileHeight = _height;

    // You may want to resize your canvas or adjust other view parameters
    // based on the new tile size
}

void TE_TileMapView::DrawTileMapGrid()
{
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // Handle zooming with mouse wheel
    if (ImGui::IsWindowHovered())
    {
        float wheel_delta = io.MouseWheel;
        if (wheel_delta != 0.f)
        {
            m_ZoomScale = std::clamp(m_ZoomScale + wheel_delta * 0.1f, 0.1f, 10.f);
        }
    }

    // Handle panning with right-click drag
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        m_RightClickDragging = true;
        m_LastMousePos = Vec2(io.MousePos.x, io.MousePos.y);
    }
    if (m_RightClickDragging)
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
        {
            Vec2 delta = Vec2(io.MousePos.x - m_LastMousePos.x, io.MousePos.y - m_LastMousePos.y);
            m_CanvasScroll.x += delta.x;
            m_CanvasScroll.y += delta.y;
            m_LastMousePos = Vec2(io.MousePos.x, io.MousePos.y);
        }
        else
        {
            m_RightClickDragging = false;
        }
    }

    // Calculate canvas size and position
    ImVec2 window_pos = ImGui::GetCursorScreenPos();
    ImVec2 window_size = ImGui::GetContentRegionAvail();
    ImVec2 canvas_p0 = window_pos;
    ImVec2 canvas_p1 = ImVec2(window_pos.x + window_size.x, window_pos.y + window_size.y);

    // Draw canvas background
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(30, 30, 30, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // Get the tileset selected in TileSetView
    Ptr<DTexture> tileset = GetTileSetView()->GetTilesetTexture();
    if (!tileset)
    {
        ImGui::Text("No tileset loaded");
        return;
    }

    // Calculate scaled tile size
    float scaled_tile_width = m_TileWidth * m_ZoomScale;
    float scaled_tile_height = m_TileHeight * m_ZoomScale;

    // Calculate origin point for grid
    ImVec2 origin = ImVec2(
        canvas_p0.x + m_CanvasScroll.x,
        canvas_p0.y + m_CanvasScroll.y
    );

    // Draw grid
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    for (int row = 0; row < m_NumRows; ++row)
    {
        for (int col = 0; col < m_NumColumns; ++col)
        {
            // Calculate tile rectangle in screen space
            ImVec2 tile_p0 = ImVec2(
                origin.x + col * scaled_tile_width,
                origin.y + row * scaled_tile_height
            );
            ImVec2 tile_p1 = ImVec2(
                tile_p0.x + scaled_tile_width,
                tile_p0.y + scaled_tile_height
            );

            // Skip if tile is outside the visible area
            if (tile_p1.x < canvas_p0.x || tile_p0.x > canvas_p1.x ||
                tile_p1.y < canvas_p0.y || tile_p0.y > canvas_p1.y)
            {
                continue;
            }

            // Get tile data
            int index = row * m_NumColumns + col;
            TileData& tile = m_Grid[index];

            // Draw the tile if it's occupied
            if (tile.IsOccupied)
            {
                // Get tileset dimensions
                float tex_width = (float)tileset->GetWidth();
                float tex_height = (float)tileset->GetHeight();

                // Get the tile's position in the tileset
                float tileUVWidth = m_TileWidth / tex_width;
                float tileUVHeight = m_TileHeight / tex_height;

                // Calculate UV coordinates for the tile
                ImVec2 uv0 = ImVec2(
                    tile.TileIndex.x * tileUVWidth,
                    tile.TileIndex.y * tileUVHeight
                );
                ImVec2 uv1 = ImVec2(
                    (tile.TileIndex.x + 1.f) * tileUVWidth,
                    (tile.TileIndex.y + 1.f) * tileUVHeight
                );

                // Draw the tile
                draw_list->AddImage(
                    (void*)tileset->GetSRV().Get(),
                    tile_p0, tile_p1,
                    uv0, uv1
                );
            }

            // Draw grid lines for all tiles
            draw_list->AddRect(tile_p0, tile_p1, IM_COL32(100, 100, 100, 200));
        }
    }
    draw_list->PopClipRect();

    // Make the canvas interactive
    ImGui::SetCursorScreenPos(window_pos);
    ImGui::InvisibleButton("tilemap_canvas", window_size);
}

void TE_TileMapView::HandleTilePlacement()
{
    ImGuiIO& io = ImGui::GetIO();

    // Only process clicks if the mouse is inside the window and left button is clicked/held
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        // Calculate which tile was clicked
        ImVec2 window_pos = ImGui::GetItemRectMin();
        ImVec2 mouse_pos = io.MousePos;

        // Calculate origin point for grid
        ImVec2 origin = ImVec2(
            window_pos.x + m_CanvasScroll.x,
            window_pos.y + m_CanvasScroll.y
        );

        // Convert mouse coordinates to tile grid coordinates
        float scaled_tile_width = m_TileWidth * m_ZoomScale;
        float scaled_tile_height = m_TileHeight * m_ZoomScale;

        int col = (int)((mouse_pos.x - origin.x) / scaled_tile_width);
        int row = (int)((mouse_pos.y - origin.y) / scaled_tile_height);

        // Ensure the selected tile is within bounds
        if (col >= 0 && col < m_NumColumns && row >= 0 && row < m_NumRows)
        {
            if (m_EraserMode)
            {
                // Remove the tile
                RemoveTile(row, col);
            }
            else
            {
                // Get the selected tile from TileSetView
                Vec2 selectedTile = GetTileSetView()->GetSelectedTile();

                // Only place tile if a valid tile is selected in TileSetView
                if (selectedTile.x >= 0 && selectedTile.y >= 0)
                {
                    // Place the tile
                    PlaceTile(row, col, selectedTile);
                }
            }
        }
    }
}
