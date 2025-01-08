#pragma once
#include "AssetUI.h"

class SpriteUI :
    public AssetUI
{
public:
    SpriteUI();
    ~SpriteUI();

public:
    virtual void Update() override;

};
