#pragma once
#include "DEditor.h"

class MenuUI :
    public DEditor
{
public:
    MenuUI();
    ~MenuUI();

public:
    virtual void Tick() override;
    virtual void Update() override;

private:
    void File();
    void Level();
    void GameObject();
    void Assets();
    void Editor();

private:
    void AddScript();

    wstring GetAssetKey(ASSET_TYPE _Type, const wstring& _Format);

};
