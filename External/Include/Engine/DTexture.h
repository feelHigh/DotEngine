#ifndef ENGINE_DTEXTURE_H
#define ENGINE_DTEXTURE_H

#include "DAsset.h"

class DTexture :
    public DAsset
{
public:
    CLONE_DISABLE(DTexture);
    DTexture();
    ~DTexture();

public:
    // _Flags : D3D11_BIND_FLAG
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _Flags, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
    int Create(ComPtr<ID3D11Texture2D> _Tex2D);

    // GracphiShader - t register
    void Binding(UINT _RegisterNum);
    static void Clear(UINT _RegisterNum);

    // ComputeShader - t register
    void Binding_CS_SRV(UINT _RegisterNum);
    void Clear_CS_SRV();

    // ComputeShader - u register
    void Binding_CS_UAV(UINT _RegisterNum);
    void Clear_CS_UAV();

public:
    ComPtr<ID3D11Texture2D>             GetTex2D() { return m_Tex2D; }
    ComPtr<ID3D11RenderTargetView>      GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView>      GetDSV() { return m_DSV; }
    ComPtr<ID3D11ShaderResourceView>    GetSRV() { return m_SRV; }
    ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return m_UAV; }
    const D3D11_TEXTURE2D_DESC&         GetDesc() { return m_Desc; }

public:
    tPixel* GetPixels() { return (tPixel*)m_Image.GetPixels(); }

public:
    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    ScratchImage                        m_Image;
    ComPtr<ID3D11Texture2D>			    m_Tex2D;

    ComPtr<ID3D11RenderTargetView>      m_RTV;
    ComPtr<ID3D11DepthStencilView>      m_DSV;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_TEXTURE2D_DESC                m_Desc;

    UINT                                m_RecentBindingRegisterNum;

};

#endif // !ENGINE_DTEXTURE_H