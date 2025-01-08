#pragma once
// Base Headers
#include "AssetList.h"
#include "Ptr.h"

// Editor Headers
#include "DEditor.h"

class Inspector :
    public DEditor
{
public:
    Inspector();
    ~Inspector();

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    void SetTargetObject(DGameObject* _Object);
    void SetTargetAsset(Ptr<DAsset> _Asset);

    DGameObject* GetTargetObject() { return m_TargetObject; }
    Ptr<DAsset> GetTargetAsset() { return m_TargetAsset; }

private:
    void CreateComponentUI();
    void CreateScriptUI(UINT _Count);
    void CreateAssetUI();

private:
    class DGameObject*      m_TargetObject;
    class ComponentUI*      m_arrComUI[(UINT)COMPONENT_TYPE::END];
    vector<class ScriptUI*> m_vecScriptUI;

    Ptr<DAsset>             m_TargetAsset;
    class AssetUI*          m_arrAssetUI[(UINT)ASSET_TYPE::END];

};

