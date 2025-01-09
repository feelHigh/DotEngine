#pragma once
#include "DEditor.h"

class SE_AtlasView;
class SE_SpriteView;
class SE_SpriteMod;

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
	SE_SpriteMod* GetSpriteMod() { return m_SpriteMod; }

private:
	SE_AtlasView*	m_AtlasView;
	SE_SpriteView*	m_SpriteView;
	SE_SpriteMod*	m_SpriteMod;

};
