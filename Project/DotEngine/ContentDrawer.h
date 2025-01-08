#pragma once
#include "DEditor.h"

class ContentDrawer :
    public DEditor
{
public:
    ContentDrawer();
    ~ContentDrawer();

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void RenewContent();
    void AssetClicked(DWORD_PTR _Param);
    void PopupMenu(DWORD_PTR _Parma);

    void FindAssetName(const wstring& _FolderPath, const wstring& _Filter);
    void LoadAsset(const path& _Path);

public:
    // Content 폴더에 있는 리소스(에셋) 들을 확인 및 로딩
    void Reload();

private:
    class TreeUI* m_Tree;
    vector<wstring> m_vecAssetPath; // Content 폴더에 있는 에셋 경로

};
