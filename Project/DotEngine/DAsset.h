#ifndef ENGINE_DASSET_H
#define ENGINE_DASSET_H

#include "DEntity.h"

class DAsset :
    public DEntity
{
public:
    template<typename T>
    friend class Ptr;

    friend class DAssetMgr;
    friend class SE_SpriteView;

public:
    CLONE_DISABLE(DAsset);
    DAsset(ASSET_TYPE _Type);
    DAsset(const DAsset& _Other);
    ~DAsset();

public:
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }
    ASSET_TYPE GetAssetType() { return m_Type; }
    UINT GetRefCount() { return m_RefCount; }
    bool IsEngineAsset() { return m_Engine; }

protected:
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _path) { m_RelativePath = _path; }

protected:
    void SetEngineAsset() { m_Engine = true; }

private:
    virtual int Load(const wstring& _FilePath) = 0;
    virtual int Save(const wstring& _RelativePath) = 0;

private:
    void AddRef() { ++m_RefCount; }
    void Release()
    {
        --m_RefCount;
        if (m_RefCount <= 0)
        {
            delete this;
        }
    }

private:
    wstring             m_Key;
    wstring             m_RelativePath;
    const ASSET_TYPE    m_Type;
    int                 m_RefCount;

    bool                m_Engine;
};

#endif // !ENGINE_DASSET_H