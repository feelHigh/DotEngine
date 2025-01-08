#pragma once
#include "AssetUI.h"

class SoundUI :
    public AssetUI
{
public:
    SoundUI();
    ~SoundUI();

public:
    virtual void Update() override;

};

