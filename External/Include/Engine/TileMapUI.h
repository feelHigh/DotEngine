#pragma once
#include "ComponentUI.h"

class TileMapUI :
    public ComponentUI
{
public:
    TileMapUI();
    ~TileMapUI();

public:
    virtual void Update() override;

};
