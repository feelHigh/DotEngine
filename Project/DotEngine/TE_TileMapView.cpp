#include "pch.h"
#include "TE_TileMapView.h"

TE_TileMapView::TE_TileMapView()
    : m_TileTex(nullptr)
    , m_TileWidth(32)
    , m_TileHeight(32)
    , m_NumRows(9)
    , m_NumColumns(9)
    , m_ZoomScale(1.0f)
    , m_CanvasScroll(ImVec2(0.f, 0.f))
{
    m_Grid.resize(m_NumRows, std::vector<ImVec2>(m_NumColumns, ImVec2(-1, -1)));
}

TE_TileMapView::~TE_TileMapView()
{
}

void TE_TileMapView::Init()
{
}

void TE_TileMapView::Update()
{
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Define canvas area
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz(m_NumColumns * m_TileWidth * m_ZoomScale, m_NumRows * m_TileHeight * m_ZoomScale);
    ImVec2 canvas_p1 = canvas_p0 + canvas_sz;

    // Draw background
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255)); // Background
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));   // Border

    // Invisible button to catch interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered();
    const bool is_active = ImGui::IsItemActive();
    const ImVec2 origin(canvas_p0.x + m_CanvasScroll.x, canvas_p0.y + m_CanvasScroll.y);

    // Mouse position within the canvas
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

    // Zoom control using mouse wheel
    float wheel_delta = io.MouseWheel;
    if (wheel_delta != 0.0f && is_hovered)
    {
        m_ZoomScale += wheel_delta * 0.1f;
        m_ZoomScale = std::clamp(m_ZoomScale, 0.1f, 10.0f); // Clamp scale between 10% and 1000%
    }

    // Handle panning (right mouse drag)
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
    {
        m_CanvasScroll.x += io.MouseDelta.x;
        m_CanvasScroll.y += io.MouseDelta.y;
    }

    draw_list->PushClipRect(canvas_p0, canvas_p1, true);

    // Draw grid and tiles
    for (int row = 0; row < m_NumRows; ++row)
    {
        for (int col = 0; col < m_NumColumns; ++col)
        {
            ImVec2 tile_p0 = origin + ImVec2(col * m_TileWidth * m_ZoomScale, row * m_TileHeight * m_ZoomScale);
            ImVec2 tile_p1 = tile_p0 + ImVec2(m_TileWidth * m_ZoomScale, m_TileHeight * m_ZoomScale);

            draw_list->AddRect(tile_p0, tile_p1, IM_COL32(255, 255, 255, 255)); // Grid lines

        }
    }

    draw_list->PopClipRect();
}

void TE_TileMapView::SetTargetTile(Ptr<DTexture> _Tex, Vec2 _LeftTopPixel, Vec2 _Slice)
{
    m_TileTex = _Tex;
    m_TileLeftTopPixel = _LeftTopPixel;
    m_TileSlice = _Slice;
}

void TE_TileMapView::SetGridSize(int rows, int columns)
{
    m_NumRows = rows;
    m_NumColumns = columns;
    m_Grid.resize(m_NumRows, std::vector<ImVec2>(m_NumColumns, ImVec2(-1, -1)));
}
