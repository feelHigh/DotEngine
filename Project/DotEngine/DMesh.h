#ifndef ENGINE_DMESH_H
#define ENGINE_DMESH_H

#include "DAsset.h"

class DMesh :
    public DAsset
{
public:
    DMesh();
    ~DMesh();

public:
    int Create(tVtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount);
    void Binding();
    void Render();
    void Render_Particle(UINT _Count);

    void* GetVtxSysMem() { return m_VtxSysMem; }

public:
    virtual int Load(const wstring& _FilePath) { return S_OK; }
    virtual int Save(const wstring& _FilePath) { return S_OK; }

private:
    void* m_VtxSysMem;
    void* m_IdxSysMem;

private:
    ComPtr<ID3D11Buffer>	m_VB;
    ComPtr<ID3D11Buffer>	m_IB;

    UINT                    m_VtxCount;
    UINT                    m_IdxCount;

    D3D11_BUFFER_DESC       m_VBDesc;
    D3D11_BUFFER_DESC       m_IBDesc;

};

#endif // !ENGINE_DMESH_H