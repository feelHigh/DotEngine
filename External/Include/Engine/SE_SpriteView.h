#pragma once
#include "SE_Sub.h"

class SE_SpriteView :
	public SE_Sub
{
public:
	SE_SpriteView();
	~SE_SpriteView();

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	void SetTargetSprite(Ptr<DTexture> _Tex, Vec2 _LeftTopPixel, Vec2 _Slice);
	void SetSpriteSize(int width, int height);

private:
	Ptr<DTexture>   m_AtlasTex;

	int				m_SpriteWidth;	// Sprite width (from AtlasView)
	int				m_SpriteHeight;	// Sprite height (from AtlasView)
	int				m_OffsetX;		// X offset
	int				m_OffsetY;		// Y offset

	float			m_ZoomScale;	// Initialize zoom level

	Vec2			m_LeftTopPixel;
	Vec2			m_Slice;

	ImVec2			m_CanvasScroll;	// For panning

};
