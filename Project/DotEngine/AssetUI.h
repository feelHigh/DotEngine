#pragma once
#include "DEditor.h"

class AssetUI :
    public DEditor
{
public:
    AssetUI(ASSET_TYPE _Type);
    ~AssetUI();

public:
    void SetAsset(Ptr<DAsset> _Asset);
    Ptr<DAsset> GetAsset() { return m_Asset; }

protected:
    void Title();
    void OutputAssetName();

private:
    Ptr<DAsset>         m_Asset;
    const ASSET_TYPE    m_Type;

};
