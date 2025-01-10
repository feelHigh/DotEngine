#ifndef ENGINE_DSPRITE_H
#define ENGINE_DSPRITE_H

#include "DAsset.h"

class DSprite :
    public DAsset
{
public:
    DSprite();
    ~DSprite();

public:
    void Create(Ptr<DTexture> _Atlas, Vec2 _LeftTopPixel, Vec2 _Slice);

public:
    Ptr<DTexture> GetAtlasTexture() { return m_Atlas; }

    void SetLeftTop(Vec2 _LeftTop);
    void SetSlice(Vec2 _Slice);
    void SetBackground(Vec2 _Background);
    void SetOffset(Vec2 _Offset);

    Vec2 GetLeftTopUV() { return m_LeftTopUV; }
    Vec2 GetSliceUV() { return m_SliceUV; }
    Vec2 GetBackgroundUV() { return m_BackgroundUV; }
    Vec2 GetOffsetUV() { return m_OffsetUV; }

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    Ptr<DTexture>   m_Atlas;
    Vec2            m_LeftTopUV;      // UV ÁÂÇ¥°è
    Vec2            m_SliceUV;        // UV ÁÂÇ¥°è
    Vec2            m_BackgroundUV;
    Vec2            m_OffsetUV;

};

#endif // !ENGINE_DSPRITE_H