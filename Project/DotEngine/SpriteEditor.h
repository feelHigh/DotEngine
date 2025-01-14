#pragma once
#include "DEditor.h"

class SE_AtlasView;
class SE_SpriteView;
class SE_SpriteDisplay;

class SpriteEditor
	: public DEditor
{
public:
	SpriteEditor();
	~SpriteEditor();

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	virtual void Activate() override;
	virtual void Deactivate() override;

public:
	SE_AtlasView* GetAtlasView() { return m_AtlasView; }
	SE_SpriteView* GetSpriteView() { return m_SpriteView; }
	SE_SpriteDisplay* GetSpriteDisplay() { return m_SpriteDisplay; }

private:
	SE_AtlasView*		m_AtlasView;
	SE_SpriteView*		m_SpriteView;
	SE_SpriteDisplay*	m_SpriteDisplay;

};
