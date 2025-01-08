#pragma once
#include "AssetUI.h"

class ComputeShaderUI :
    public AssetUI
{
public:
    ComputeShaderUI();
    ~ComputeShaderUI();

public:
    virtual void Update() override;

};
