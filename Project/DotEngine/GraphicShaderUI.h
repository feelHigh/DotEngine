#pragma once
#include "AssetUI.h"

class GraphicShaderUI :
    public AssetUI
{
public:
    GraphicShaderUI();
    ~GraphicShaderUI();

public:
    virtual void Update() override;

};
