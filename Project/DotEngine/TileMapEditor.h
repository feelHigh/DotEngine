#pragma once

#include "DEditor.h"

class TE_TileSetView;
class TE_TileMapView;
class TE_TileMapMod;

class TileMapEditor :
    public DEditor
{
public:
    TileMapEditor();
    ~TileMapEditor();

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    TE_TileSetView* GetTileSetView() { return m_TileSetView; }
    TE_TileMapView* GetTileMapView() { return m_TileMapView; }
    TE_TileMapMod* GetTileMapMod() { return m_TileMapMod; }

public:
    virtual void Activate() override;
    virtual void Deactivate() override;

private:
    TE_TileSetView* m_TileSetView;
    TE_TileMapView* m_TileMapView;
    TE_TileMapMod* m_TileMapMod;

};
