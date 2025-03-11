#include "pch.h"
#include "TE_TileSetView.h"
#include "DAssetMgr.h"

TE_TileSetView::TE_TileSetView()
    : m_TilesetTexture(nullptr)
    , m_SelectedTile(Vec2(-1.f, -1.f))
    , m_TileWidth(32.f)
    , m_TileHeight(32.f)
    , m_Rows(1)
    , m_Columns(1)
    , m_WheelScale(1.f)
    , m_CanvasScroll(Vec2(0.f, 0.f))
    , m_RightClickDragging(false)
    , m_LastMousePos(Vec2(0.f, 0.f))
{
}

TE_TileSetView::~TE_TileSetView()
{
}

void TE_TileSetView::Init()
{
    // Initialize default tileset texture (can be loaded later by user)
    m_TilesetTexture = DAssetMgr::GetInst()->FindAsset<DTexture>(L"DefaultTileset");
    if (nullptr == m_TilesetTexture)
    {
        // Create a default empty texture if none exists
        m_TilesetTexture = new DTexture;
        m_TilesetTexture->Create(32, 32, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
        DAssetMgr::GetInst()->AddAsset(L"DefaultTileset", m_TilesetTexture);
    }
}

void TE_TileSetView::Update()
{
    // Controls for tileset parameters
    ImGui::Text("Tileset Settings");
    ImGui::InputFloat("Tile Width", &m_TileWidth, 1.0f, 10.0f);
    ImGui::InputFloat("Tile Height", &m_TileHeight, 1.0f, 10.0f);
    ImGui::InputInt("Rows", &m_Rows);
    ImGui::InputInt("Columns", &m_Columns);
    m_TileWidth = max(1.f, m_TileWidth);
    m_TileHeight = max(1.f, m_TileHeight);
    m_Rows = max(1, m_Rows);
    m_Columns = max(1, m_Columns);

    // Button to load a tileset texture
    if (ImGui::Button("Load Tileset"))
    {
        // Open file dialog to load a tileset texture
        // For now, just use a placeholder
        m_TilesetTexture = DAssetMgr::GetInst()->FindAsset<DTexture>(L"DefaultTileset");
    }

    // Display tileset and handle tile selection
    if (m_TilesetTexture != nullptr)
    {
        DrawTilesetGrid();
        HandleTileSelection();
    }
    else
    {
        ImGui::Text("No tileset texture loaded.");
    }
}

void TE_TileSetView::DrawTilesetGrid()
{
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Handle zooming with mouse wheel
    if (ImGui::IsWindowHovered())
    {
        float wheel_delta = io.MouseWheel;
        if (wheel_delta != 0.f)
        {
            m_WheelScale = std::clamp(m_WheelScale + wheel_delta * 0.1f, 0.1f, 10.f);
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
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // Get tileset dimensions
    float tex_width = (float)m_TilesetTexture->GetWidth();
    float tex_height = (float)m_TilesetTexture->GetHeight();

    // Calculate scaled tile size
    float scaled_tile_width = m_TileWidth * m_WheelScale;
    float scaled_tile_height = m_TileHeight * m_WheelScale;

    // Draw grid
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);

    for (int row = 0; row < m_Rows; ++row)
    {
        for (int col = 0; col < m_Columns; ++col)
        {
            // Calculate tile rectangle in screen space
            ImVec2 tile_p0 = ImVec2(
                canvas_p0.x + m_CanvasScroll.x + col * scaled_tile_width,
                canvas_p0.y + m_CanvasScroll.y + row * scaled_tile_height
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

            // Calculate UV coordinates for the tile
            ImVec2 uv0 = ImVec2(
                (float)col * m_TileWidth / tex_width,
                (float)row * m_TileHeight / tex_height
            );
            ImVec2 uv1 = ImVec2(
                (float)(col + 1) * m_TileWidth / tex_width,
                (float)(row + 1) * m_TileHeight / tex_height
            );

            // Draw the tile
            draw_list->AddImage(
                (void*)m_TilesetTexture->GetSRV().Get(),
                tile_p0, tile_p1,
                uv0, uv1
            );

            // Draw grid lines
            draw_list->AddRect(tile_p0, tile_p1, IM_COL32(200, 200, 200, 100));

            // Highlight selected tile
            if (m_SelectedTile.x == col && m_SelectedTile.y == row)
            {
                draw_list->AddRect(tile_p0, tile_p1, IM_COL32(255, 255, 0, 255), 0.f, 0, 2.f);
            }
        }
    }

    draw_list->PopClipRect();

    // Make the canvas interactive
    ImGui::SetCursorScreenPos(window_pos);
    ImGui::InvisibleButton("tileset_canvas", window_size);
}

void TE_TileSetView::HandleTileSelection()
{
    ImGuiIO& io = ImGui::GetIO();

    // Only process clicks if the mouse is inside the window and left button is clicked
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        // Calculate which tile was clicked
        ImVec2 window_pos = ImGui::GetItemRectMin();
        ImVec2 mouse_pos = io.MousePos;

        // Convert mouse coordinates to tile grid coordinates
        float scaled_tile_width = m_TileWidth * m_WheelScale;
        float scaled_tile_height = m_TileHeight * m_WheelScale;

        int col = (int)((mouse_pos.x - window_pos.x - m_CanvasScroll.x) / scaled_tile_width);
        int row = (int)((mouse_pos.y - window_pos.y - m_CanvasScroll.y) / scaled_tile_height);

        // Ensure the selected tile is within bounds
        if (col >= 0 && col < m_Columns && row >= 0 && row < m_Rows)
        {
            m_SelectedTile = Vec2((float)col, (float)row);
        }
    }
}
