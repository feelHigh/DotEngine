#include "pch.h"
#include "SE_SpriteView.h"
#include "DAssetMgr.h"

SE_SpriteView::SE_SpriteView()
    : m_AtlasTex(nullptr)
    , m_SpriteLeftTop(0)
    , m_SpriteWidth(0)
    , m_SpriteHeight(0)
    , m_OffsetX(0)
    , m_OffsetY(0)
    , m_ZoomScale(8.57f)
    , m_Slice(0, 0)
    , m_CanvasScroll(ImVec2(0.f, 0.f))
{
}

SE_SpriteView::~SE_SpriteView()
{
}

void SE_SpriteView::Init()
{
}

void SE_SpriteView::Update()
{
    if (!m_AtlasTex)
        return;

    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Define canvas area
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // Top-left corner of the canvas
    ImVec2 canvas_size = ImVec2(600, 600);          // Set a fixed canvas size
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_size.x, canvas_p0.y + canvas_size.y);     // Bottom-right corner

    // Draw background
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(30, 30, 30, 255)); // Dark gray background
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));   // White border

    // Invisible button to catch interactions
    ImGui::InvisibleButton("canvas", canvas_size, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
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

    // Clip drawing to the canvas area
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);

    // Draw grid (scaled by zoom level)
    float grid_size = m_ZoomScale; // Each grid cell represents one pixel
    ImU32 grid_color = IM_COL32(50, 50, 50, 255);

    for (float x = canvas_p0.x + m_CanvasScroll.x; x < canvas_p1.x; x += grid_size)
        draw_list->AddLine(ImVec2(x, canvas_p0.y), ImVec2(x, canvas_p1.y), grid_color);
    for (float y = canvas_p0.y + m_CanvasScroll.y; y < canvas_p1.y; y += grid_size)
        draw_list->AddLine(ImVec2(canvas_p0.x, y), ImVec2(canvas_p1.x, y), grid_color);

    // Draw sprite (scaled and centered, offset applied)
    ImVec2 sprite_pos = ImVec2(canvas_p0.x + m_CanvasScroll.x + m_OffsetX * m_ZoomScale
                            , canvas_p0.y + m_CanvasScroll.y + m_OffsetY * m_ZoomScale);
    ImVec2 sprite_size = ImVec2(m_Slice.x * m_ZoomScale, m_Slice.y * m_ZoomScale);
    ImVec2 uv0 = ImVec2(m_LeftTopPixel.x / m_AtlasTex->GetWidth(), m_LeftTopPixel.y / m_AtlasTex->GetHeight());
    ImVec2 uv1 = ImVec2((m_LeftTopPixel.x + m_Slice.x) / m_AtlasTex->GetWidth()
                       ,(m_LeftTopPixel.y + m_Slice.y) / m_AtlasTex->GetHeight());

    draw_list->AddImage(m_AtlasTex->GetSRV().Get(), sprite_pos
                        , ImVec2(sprite_pos.x + sprite_size.x, sprite_pos.y + sprite_size.y)
                        , uv0, uv1);

    // Calculate highlight square (independent of offset)
    ImVec2 highlight_pos = ImVec2(canvas_p0.x + m_CanvasScroll.x, canvas_p0.y + m_CanvasScroll.y);
    ImVec2 highlight_size = ImVec2(m_SpriteWidth * m_ZoomScale, m_SpriteHeight * m_ZoomScale);
    ImVec2 highlight_bottom_right = ImVec2(highlight_pos.x + highlight_size.x, highlight_pos.y + highlight_size.y);

    draw_list->AddRect(highlight_pos, highlight_bottom_right, IM_COL32(255, 0, 0, 255), 0.0f, 0, 1.0f);

    // Draw center axis within the highlight square
    ImU32 axis_color = IM_COL32(0, 255, 0, 255); // Bright green for the axes
    float axis_thickness = 1.0f;

    ImVec2 highlight_center = ImVec2(highlight_pos.x + highlight_size.x * 0.5f, highlight_pos.y + highlight_size.y * 0.5f);

    // Horizontal line within the square
    draw_list->AddLine(
        ImVec2(highlight_pos.x, highlight_center.y),
        ImVec2(highlight_bottom_right.x, highlight_center.y),
        axis_color, axis_thickness
    );

    // Vertical line within the square
    draw_list->AddLine(
        ImVec2(highlight_center.x, highlight_pos.y),
        ImVec2(highlight_center.x, highlight_bottom_right.y),
        axis_color, axis_thickness
    );

    draw_list->PopClipRect();

    ImGui::Text("Zoom Level: %.2f", m_ZoomScale);

    // Input fields for offset
    ImGui::Text("Offset");
    ImGui::SliderInt("Offset X", &m_OffsetX, -32, 32);
    ImGui::SliderInt("Offset Y", &m_OffsetY, -32, 32);

    SaveSprite();
}

void SE_SpriteView::SetTargetSprite(Ptr<DTexture> _Tex, Vec2 _LeftTopPixel, Vec2 _Slice)
{
    m_AtlasTex = _Tex;
    m_LeftTopPixel = _LeftTopPixel;
    m_Slice = _Slice;
}

void SE_SpriteView::SetSpriteSize(int width, int height)
{
    m_SpriteWidth = width;
    m_SpriteHeight = height;
}

void SE_SpriteView::SaveSprite()
{
    wstring strContentPath = DPathMgr::GetInst()->GetContentPath();

    // Static variable to hold the file name
    static char fileName[256] = "default_name";

    // Input text for the file name
    ImGui::Text("File Name");
    ImGui::InputText("##FileNameInput", fileName, IM_ARRAYSIZE(fileName));

    // Convert char to wstring for save
    wstring filePath = wstring(fileName, fileName + strlen(fileName));

    // Add the Save button
    ImGui::Text("Save Sprite");
    if (ImGui::Button("Save"))
    {
        Ptr<DSprite> pTargetSave = new DSprite;
        pTargetSave->Create(m_AtlasTex, m_LeftTopPixel, m_Slice);
        pTargetSave->SetBackground(Vec2(m_SpriteWidth, m_SpriteHeight));

        // Set the offset values
        pTargetSave->SetOffset(Vec2((float)m_OffsetX, (float)m_OffsetY));
        pTargetSave->SetRelativePath(wstring(L"Sprite\\") + filePath + L".sprite");

        // Save the sprite
        if (pTargetSave->Save(strContentPath + L"Sprite\\" + filePath + L".sprite") == S_OK)
        {
            pTargetSave = DAssetMgr::GetInst()->Load<DSprite>(filePath, wstring(L"Sprite\\") + filePath + L".sprite");
            ImGui::Text("Sprite saved successfully!");
        }
        else
        {
            ImGui::Text("Failed to save sprite.");
        }
    }

    ImGui::Text("Create Flipbook");
    if (ImGui::Button("Create"))
    {
        Ptr<DFlipbook> pFlipbook = new DFlipbook;

        for (int i = 0; i < 8; ++i)
        {
            wchar_t Buffer[50] = {};
            swprintf_s(Buffer, 50, L"Idle_Left_%d", i);
            pFlipbook->AddSprite(DAssetMgr::GetInst()->FindAsset<DSprite>(Buffer));
        }

        DAssetMgr::GetInst()->AddAsset(L"Idle_Left", pFlipbook);
        pFlipbook->Save(strContentPath + L"Flipbook\\" + L"Idle_Left" + L".flip");
    }
}
