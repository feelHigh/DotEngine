#pragma once
#include "AssetUI.h"

class TextureUI :
    public AssetUI
{
public:
    TextureUI();
    ~TextureUI();

public:
    virtual void Update() override;

};
