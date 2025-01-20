#pragma once
#include "DEditor.h"

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


};
