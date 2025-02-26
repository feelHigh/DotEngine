#include "pch.h"
#include "TE_TileSetView.h"

#include "SE_SpriteView.h"

// New variables for splitting settings
static int columns = 3;                // Number of columns
static int rows = 3;                   // Number of rows
static int sprite_width = 32;          // Sprite width (default)
static int sprite_height = 32;         // Sprite height (default)
static ImVec2 selected_sprite(-1, -1); // Selected sprite (row, column)

// Highlight and selection colors
static ImU32 highlight_color = IM_COL32(255, 255, 255, 10);    // Light blue
static ImU32 selection_color = IM_COL32(255, 255, 255, 50);      // Deeper blue
static ImU32 border_color = IM_COL32(146, 190, 200, 255);       // Black border for highlight

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

    // Add ImGui inputs for sprite sheet splitting
    ImGui::Text("TileSet");
    ImGui::InputInt("Columns", &columns);
    ImGui::InputInt("Rows", &rows);
    ImGui::InputInt("Tile Width", &sprite_width);
    ImGui::InputInt("Tile Height", &sprite_height);

    // Validate inputs
    columns = max(columns, 1);
    rows = max(rows, 1);
    sprite_width = max(sprite_width, 1);
    sprite_height = max(sprite_height, 1);
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
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
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
            // Calculate sprite rectangle
            ImVec2 sprite_p0 = ImVec2(
                canvas_p0.x + col * (sprite_width * m_WheelScale),
                canvas_p0.y + row * (sprite_height * m_WheelScale)
            );
            ImVec2 sprite_p1 = ImVec2(
                sprite_p0.x + (sprite_width * m_WheelScale),
                sprite_p0.y + (sprite_height * m_WheelScale)
            );

            // Determine color for the rectangle
            ImU32 rect_color = highlight_color;
            if (selected_sprite.x == row && selected_sprite.y == col)
                rect_color = selection_color;

            // Draw the filled rectangle (highlight)
            draw_list->AddRectFilled(sprite_p0, sprite_p1, rect_color);

            // Draw the border
            draw_list->AddRect(sprite_p0, sprite_p1, border_color);

            // Check for mouse click to select the sprite
            if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                if (io.MousePos.x >= sprite_p0.x && io.MousePos.x <= sprite_p1.x &&
                    io.MousePos.y >= sprite_p0.y && io.MousePos.y <= sprite_p1.y)
                {
                    selected_sprite = ImVec2(row, col);
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
