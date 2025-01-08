#pragma once
#include "AssetUI.h"

class FlipbookUI :
    public AssetUI
{
public:
    FlipbookUI();
    ~FlipbookUI();

public:
    virtual void Update() override;

};
