#pragma once
#include "DEditor.h"

class TE_TileMapView;
class TE_TileMapMod;
class TE_TileSetView;

class TileMapEditor
	: public DEditor
{
public:
	TileMapEditor();
	~TileMapEditor();

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	virtual void Activate() override;
	virtual void Deactivate() override;

public:
	TE_TileMapView* GetTileMapView() { return m_TileMapView; }
	TE_TileMapMod* GetTileMapMod() { return m_TileMapMod; }
	TE_TileSetView* GetTileSetView() { return m_TileSetView; }

private:
	TE_TileMapView* m_TileMapView;
	TE_TileMapMod* m_TileMapMod;
	TE_TileSetView* m_TileSetView;

};
