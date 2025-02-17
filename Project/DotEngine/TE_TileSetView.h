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
    void SetTileSetTex(Ptr<DTexture> _Tex);

private:
    Ptr<DTexture>   m_TileSetTex;

    float           m_WidthSize;
    float           m_WheelScale;

};
