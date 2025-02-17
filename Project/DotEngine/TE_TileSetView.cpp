#include "pch.h"
#include "TE_TileSetView.h"

#include "TE_TileMapView.h"

// New variables for splitting settings
static int columns = 3;                 // Number of columns
static int rows = 3;                    // Number of rows
static int tile_width = 32;             // Tile width (default)
static int tile_height = 32;            // Tile height (default)
static ImVec2 selected_tile(-1, -1);    // Selected tile (row, column)

// Highlight and selection colors
//static ImU32 highlight_color2 = IM_COL32(173, 216, 230, 100);    // Light blue
//static ImU32 selection_color2 = IM_COL32(0, 191, 255, 150);      // Deeper blue
static ImU32 highlight_color2 = IM_COL32(0, 0, 0, 25);
static ImU32 selection_color2 = IM_COL32(0, 0, 0, 0);
static ImU32 border_color2 = IM_COL32(255, 255, 255, 127);

TE_TileSetView::TE_TileSetView()
    : m_TileSetTex(nullptr)
    , m_WidthSize(200)
    , m_WheelScale(1.f)
{
}

TE_TileSetView::~TE_TileSetView()
{
}

void TE_TileSetView::Init()
{
}

void TE_TileSetView::Update()
{
    if (nullptr == m_TileSetTex)
        return;

    static ImVec2 v_CanvasScroll(0.f, 0.f);
    static bool b_EnableContextMenu = true;

    // Add ImGui inputs for tile sheet splitting
    ImGui::Text("Tile Set");
    ImGui::InputInt("Columns", &columns);
    ImGui::InputInt("Rows", &rows);
    ImGui::InputInt("Tile Width", &tile_width);
    ImGui::InputInt("Tile Height", &tile_height);

    // Validate inputs
    columns = max(columns, 1);
    rows = max(rows, 1);
    tile_width = max(tile_width, 1);
    tile_height = max(tile_height, 1);
    //-----
    // Calculate the image's scaled dimensions
    ImVec2 scaled_size = ImVec2(
        m_WidthSize * m_WheelScale,
        m_WidthSize * m_WheelScale * m_TileSetTex->GetHeight() / m_TileSetTex->GetWidth()
    );

    // Dynamically set canvas size to match the scaled image size
    ImVec2 canvas_sz = scaled_size;

    // Clamp canvas size to minimum size
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;

    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();  // Canvas top-left corner
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y); // Canvas bottom-right corner

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255)); // Background
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));   // Border

    // Invisible button to catch interactions
    ImGui::InvisibleButton("canvas2", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered();
    const bool is_active = ImGui::IsItemActive();
    const ImVec2 origin(canvas_p0.x + v_CanvasScroll.x, canvas_p0.y + v_CanvasScroll.y);

    // Mouse position within the canvas
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

    // Zoom control using mouse wheel
    float wheel_delta = io.MouseWheel;
    if (wheel_delta != 0.0f && is_hovered)
    {
        m_WheelScale += wheel_delta * 0.1f;
        m_WheelScale = std::clamp(m_WheelScale, 0.1f, 10.0f); // Clamp scale between 10% and 1000%
    }

    // Pan using right mouse button
    const float mouse_threshold_for_pan = b_EnableContextMenu ? -1.0f : 0.0f;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
    {
        v_CanvasScroll.x += io.MouseDelta.x;
        v_CanvasScroll.y += io.MouseDelta.y;
    }

    // Draw scaled image
    draw_list->AddImage(m_TileSetTex->GetSRV().Get(), canvas_p0, ImVec2((canvas_p0.x + scaled_size.x), (canvas_p0.y + scaled_size.y)));

    // Draw grid
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            // Calculate tile rectangle
            ImVec2 tile_p0 = ImVec2(
                canvas_p0.x + col * (tile_width * m_WheelScale),
                canvas_p0.y + row * (tile_height * m_WheelScale)
            );
            ImVec2 tile_p1 = ImVec2(
                tile_p0.x + (tile_width * m_WheelScale),
                tile_p0.y + (tile_height * m_WheelScale)
            );

            // Determine color for the rectangle
            ImU32 rect_color = highlight_color2;
            if (selected_tile.x == row && selected_tile.y == col)
                rect_color = selection_color2;

            // Draw the filled rectangle (highlight)
            draw_list->AddRectFilled(tile_p0, tile_p1, rect_color);

            // Draw the border
            draw_list->AddRect(tile_p0, tile_p1, border_color2);

            // Check for mouse click to select the tile
            if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                if (io.MousePos.x >= tile_p0.x && io.MousePos.x <= tile_p1.x &&
                    io.MousePos.y >= tile_p0.y && io.MousePos.y <= tile_p1.y)
                {
                    selected_tile = ImVec2(row, col);
                    GetTileMapView()->SetTargetTile(m_TileSetTex, Vec2(col * tile_width, row * tile_height), Vec2(tile_width, tile_height));
                }
            }
        }
    }

    draw_list->PopClipRect();
}

void TE_TileSetView::SetTileSetTex(Ptr<DTexture> _Tex)
{
    if (m_TileSetTex == _Tex)
        return;

    m_TileSetTex = _Tex;

    m_WidthSize = (float)m_TileSetTex->GetWidth();
}

