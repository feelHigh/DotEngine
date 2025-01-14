#pragma once
#include "DEditor.h"

#include "SpriteEditor.h"
#include "DAssetMgr.h"

class SE_Sub :
    public DEditor
{
public:
    friend class SpriteEditor;

public:
    SE_Sub();
    ~SE_Sub();

public:
    class SE_AtlasView* GetAtlasView() { return m_Owner->GetAtlasView(); }
    class SE_SpriteView* GetSpriteView() { return m_Owner->GetSpriteView(); }
    class SE_SpriteDisplay* GetSpriteDisplay() { return m_Owner->GetSpriteDisplay(); }

    SpriteEditor* GetOwner() { return m_Owner; }

private:
    SpriteEditor* m_Owner;

};
