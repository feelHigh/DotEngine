#pragma once
#include "DEditor.h"

#include "TileMapEditor.h"
#include "DAssetMgr.h"

class TE_Sub :
    public DEditor
{
public:
    friend class TileMapEditor;

public:
    TE_Sub();
    ~TE_Sub();

public:
    class TE_TileMapView* GetTileMapView() { return m_Owner->GetTileMapView(); }
    class TE_TileMapMod* GetTileMapMod() { return m_Owner->GetTileMapMod(); }
    class TE_TileSetView* GetTileSetView() { return m_Owner->GetTileSetView(); }

    TileMapEditor* GetOwner() { return m_Owner; }

private:
    TileMapEditor* m_Owner;

};
