#pragma once
#include "TE_Sub.h"

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
    void SetTargetTile(Ptr<DTexture> _Tex, Vec2 _LeftTopPixel, Vec2 _Slice);
    void SetGridSize(int rows, int columns);

private:
    Ptr<DTexture> m_TileTex;          // Current tile texture
    Vec2 m_TileLeftTopPixel;          // Top-left pixel of the selected tile
    Vec2 m_TileSlice;                 // Width and height of the selected tile

    int m_TileWidth;                  // Width of each grid tile
    int m_TileHeight;                 // Height of each grid tile

    int m_NumRows;                    // Number of rows in the grid
    int m_NumColumns;                 // Number of columns in the grid

    float m_ZoomScale;                // Zoom factor
    ImVec2 m_CanvasScroll;            // Panning offset

    std::vector<std::vector<ImVec2>> m_Grid; // Grid data
};

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs);
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs);
inline ImVec2 operator*(const ImVec2& lhs, float scalar);
inline ImVec2 operator/(const ImVec2& lhs, float scalar);
inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs);
inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs);
inline ImVec2& operator*=(ImVec2& lhs, float scalar);
inline ImVec2& operator/=(ImVec2& lhs, float scalar);

// Addition
inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

// Subtraction
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
{
    return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

// Multiplication by scalar
inline ImVec2 operator*(const ImVec2& lhs, float scalar)
{
    return ImVec2(lhs.x * scalar, lhs.y * scalar);
}

// Division by scalar
inline ImVec2 operator/(const ImVec2& lhs, float scalar)
{
    return ImVec2(lhs.x / scalar, lhs.y / scalar);
}

// Compound addition
inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

// Compound subtraction
inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

// Compound multiplication by scalar
inline ImVec2& operator*=(ImVec2& lhs, float scalar)
{
    lhs.x *= scalar;
    lhs.y *= scalar;
    return lhs;
}

// Compound division by scalar
inline ImVec2& operator/=(ImVec2& lhs, float scalar)
{
    lhs.x /= scalar;
    lhs.y /= scalar;
    return lhs;
}