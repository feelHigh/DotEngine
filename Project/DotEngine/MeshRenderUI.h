#pragma once
#include "ComponentUI.h"

class MeshRenderUI :
    public ComponentUI
{
public:
    MeshRenderUI();
    ~MeshRenderUI();

public:
    void SelectMesh(DWORD_PTR _AssetName);
    void SelectMaterial(DWORD_PTR _AssetName);

private:
    virtual void Update() override;

};
