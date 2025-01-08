#pragma once
#include "AssetUI.h"

class MeshUI :
    public AssetUI
{
public:
    MeshUI();
    ~MeshUI();

public:
    virtual void Update() override;

};
