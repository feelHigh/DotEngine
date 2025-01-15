#include "pch.h"
#include "SE_SpriteDisplay.h"

#include "DAssetMgr.h"
#include "DSprite.h"
#include "DTexture.h"
#include "SE_SpriteView.h"

SE_SpriteDisplay::SE_SpriteDisplay()
{
}

SE_SpriteDisplay::~SE_SpriteDisplay()
{
}

void SE_SpriteDisplay::Init() 
{
}

void SE_SpriteDisplay::Update()
{
    DisplaySprites();
}

void SE_SpriteDisplay::DisplaySprites()
{
    const auto& sprites = DAssetMgr::GetInst()->GetAssets(ASSET_TYPE::SPRITE);

    if (sprites.empty())
    {
        ImGui::Text("No sprites found.");
        return;
    }

    ImGui::Text("Available Sprites:");
    int count = 0;
    const int spritesPerRow = 8; // Number of sprites per row
    const float thumbnailSize = 64.0f;

    for (const auto& pair : sprites)
    {
        Ptr<DSprite> sprite = dynamic_cast<DSprite*>(pair.second.Get());
        if (!sprite)
            continue;

        Ptr<DTexture> atlas = sprite->GetAtlasTexture();
        Vec2 leftTopUV = sprite->GetLeftTopUV();
        Vec2 sliceUV = sprite->GetSliceUV();

        ImGui::PushID(count);
        if (atlas != nullptr)
        {
            ImVec2 spriteSize = ImVec2(sliceUV.x * atlas->GetWidth(), sliceUV.y * atlas->GetHeight());
            ImVec2 uv0 = ImVec2(leftTopUV.x, leftTopUV.y);
            ImVec2 uv1 = ImVec2(leftTopUV.x + sliceUV.x, leftTopUV.y + sliceUV.y);

            ImVec2 cursorPos = ImGui::GetCursorScreenPos();
            ImVec2 displaySize = ImVec2(thumbnailSize, thumbnailSize);

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddImage(
                atlas->GetSRV().Get(),
                cursorPos,
                ImVec2(cursorPos.x + displaySize.x, cursorPos.y + displaySize.y),
                uv0,
                uv1
            );

            // Add interaction
            if (ImGui::InvisibleButton("##sprite", displaySize))
            {
                // Handle selection logic (e.g., send to SE_SpriteView)
                GetSpriteView()->SetTargetSprite(atlas,
                                                Vec2(leftTopUV.x * atlas->GetWidth(), leftTopUV.y * atlas->GetHeight()),
                                                Vec2(sliceUV.x * atlas->GetWidth(), sliceUV.y * atlas->GetHeight()));
            }

            if (ImGui::IsItemHovered())
            {
                drawList->AddRect(
                    cursorPos,
                    ImVec2(cursorPos.x + displaySize.x, cursorPos.y + displaySize.y),
                    IM_COL32(255, 255, 0, 255) // Highlight border color
                );
            }
        }
        ImGui::PopID();

        if (++count % spritesPerRow != 0)
            ImGui::SameLine();
    }
}
