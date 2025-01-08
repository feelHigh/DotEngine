#ifndef ENGINE_DSHADER_H
#define ENGINE_DSHADER_H

#include "DAsset.h"

class DShader :
    public DAsset
{
public:
    DShader(ASSET_TYPE _Type);
    ~DShader();

public:
    virtual int Load(const wstring& _FilePath) { return S_OK; }
    virtual int Save(const wstring& _FilePath) { return S_OK; }

protected:
    ComPtr<ID3DBlob> m_ErrBlob;

};

#endif // !ENGINE_DSHADER_H