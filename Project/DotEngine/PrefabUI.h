#pragma once
#include "AssetUI.h"

class PrefabUI :
    public AssetUI
{
public:
    PrefabUI();
    ~PrefabUI();

public:
    virtual void Update() override;

};
