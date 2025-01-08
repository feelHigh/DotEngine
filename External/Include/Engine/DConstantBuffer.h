#pragma once
#include "DEntity.h"

class DConstantBuffer :
    public DEntity
{
public:
    CLONE_DISABLE(DConstantBuffer);
    DConstantBuffer();
    ~DConstantBuffer();

public:
    int Create(CB_TYPE _type, UINT _BufferSize);

    void SetData(void* _pData);
    void Binding();
    void Binding_CS();

private:
    ComPtr<ID3D11Buffer>    m_CB;
    D3D11_BUFFER_DESC       m_Desc;
    CB_TYPE                 m_CBType;

};
