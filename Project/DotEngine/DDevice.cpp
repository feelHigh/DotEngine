#include "pch.h"
#include "DDevice.h"

// Device Headers
#include "DConstantBuffer.h"

// Manager Headers
#include "DAssetMgr.h"

DDevice::DDevice()
    : m_hWnd(nullptr)
    , m_arrCB{}
    , m_Sampler{}
{
}

DDevice::~DDevice()
{
    for (UINT i = 0; i < (UINT)CB_TYPE::END; ++i)
    {
        if (nullptr != m_arrCB[i])
            delete m_arrCB[i];
    }
}

int DDevice::Init(HWND _hWnd, UINT _Width, UINT _Height)
{
    // 윈도우 핸들 및 해상도 값 저장
    m_hWnd = _hWnd;

    m_vResolution.x = (float)_Width;
    m_vResolution.y = (float)_Height;


    // Device, Context 생성
    UINT Flag = 0;

#ifdef _DEBUG
    Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
        , nullptr, Flag
        , nullptr, 0
        , D3D11_SDK_VERSION
        , m_Device.GetAddressOf(), nullptr, m_Context.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context creation failed.", L"Device, Context creation failed.", MB_OK);
        return E_FAIL;
    }

    // 스왚체인
    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"Swap Chain creation failed.", L"Swap Chain creation failed.", MB_OK);
        return E_FAIL;
    }

    // RenderTargetView, DepthStencilView
    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View creation failed.", L"View creation failed.", MB_OK);
        return E_FAIL;
    }


    // ViewPort 설정
    // 출력시킬 화면 윈도우 영역을 설정
    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = m_vResolution.x;
    viewport.Height = m_vResolution.y;

    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.f;

    CONTEXT->RSSetViewports(1, &viewport);


    if (FAILED(CreateConstBuffer()))
    {
        MessageBox(nullptr, L"Constant Buffer creation failed.", L"Constant Buffer creation failed.", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateRasterizerState()))
    {
        MessageBox(nullptr, L"Rasterizer State creation failed.", L"Rasterizer State creation failed.", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateDepthStencilState()))
    {
        MessageBox(nullptr, L"Depth Stencil State creation failed.", L"Depth Stencil State creation failed.", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateBlendState()))
    {
        MessageBox(nullptr, L"Blend State creation failed.", L"Blend State creation failed.", MB_OK);
        return E_FAIL;
    }


    if (FAILED(CreateSamplerState()))
    {
        MessageBox(nullptr, L"Sampler State creation failed.", L"Sampler State creation failed.", MB_OK);
        return E_FAIL;
    }


    return S_OK;
}

int DDevice::CreateSwapChain()
{
    // SwapChain: An object that manages a buffer for drawing on a screen and is responsible for outputting the image drawn on the buffer
    DXGI_SWAP_CHAIN_DESC Desc = {};

    Desc.BufferCount = 1;
    Desc.BufferDesc.Width = (UINT)m_vResolution.x;
    Desc.BufferDesc.Height = (UINT)m_vResolution.y;
    Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    Desc.BufferDesc.RefreshRate.Denominator = 1;
    Desc.BufferDesc.RefreshRate.Numerator = 120;
    Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    Desc.Windowed = true;
    Desc.OutputWindow = m_hWnd;
    Desc.Flags = 0;
    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;
    Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ComPtr<IDXGIDevice>     Device = nullptr;
    ComPtr<IDXGIAdapter>    Adapter = nullptr;
    ComPtr<IDXGIFactory>    Factory = nullptr;

    if (FAILED(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)Device.GetAddressOf())))
        return E_FAIL;

    if (FAILED(Device->GetParent(__uuidof(IDXGIAdapter), (void**)Adapter.GetAddressOf())))
        return E_FAIL;

    if (FAILED(Adapter->GetParent(__uuidof(IDXGIFactory), (void**)Factory.GetAddressOf())))
        return E_FAIL;

    if (FAILED(Factory->CreateSwapChain(m_Device.Get(), &Desc, m_SwapChain.GetAddressOf())))
        return E_FAIL;

    return S_OK;
}

int DDevice::CreateView()
{
    // =======================================================
    // Create Render Target Texture, Depth Stencil Texture
    // =======================================================
    // Call the Swap Chain back-buffer address
    ComPtr<ID3D11Texture2D> RenderTargetTex;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)RenderTargetTex.GetAddressOf());
    m_RTTex = DAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", RenderTargetTex);

    // Create Depth Stencil texture
    m_DSTex = DAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex"
                                                , (UINT)m_vResolution.x, (UINT)m_vResolution.y
                                                , DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

    return S_OK;
}

int DDevice::CreateConstBuffer()
{
    DConstantBuffer* pCB = nullptr;

    // Forward world, view, projection matrix
    pCB = new DConstantBuffer;
    if (FAILED(pCB->Create(CB_TYPE::TRANSFORM, sizeof(tTransform))))
    {
        MessageBox(nullptr, L"Constant Buffer creation failed.(Transform)", L"Constant Buffer creation failed.(Transform)", MB_OK);
        return E_FAIL;
    }
    m_arrCB[(UINT)CB_TYPE::TRANSFORM] = pCB;


    pCB = new DConstantBuffer;
    if (FAILED(pCB->Create(CB_TYPE::MATERIAL, sizeof(tMtrlConst))))
    {
        MessageBox(nullptr, L"Constant Buffer creation failed.(Material)", L"Constant Buffer creation failed.(Material)", MB_OK);
        return E_FAIL;
    }
    m_arrCB[(UINT)CB_TYPE::MATERIAL] = pCB;

    pCB = new DConstantBuffer;
    if (FAILED(pCB->Create(CB_TYPE::SPRITE, sizeof(tSpriteInfo))))
    {
        MessageBox(nullptr, L"Constant Buffer creation failed.(Sprite)", L"Constant Buffer creation failed.(Sprite)", MB_OK);
        return E_FAIL;
    }
    m_arrCB[(UINT)CB_TYPE::SPRITE] = pCB;


    pCB = new DConstantBuffer;
    if (FAILED(pCB->Create(CB_TYPE::GLOBAL, sizeof(tGlobalData))))
    {
        MessageBox(nullptr, L"Constant Buffer creation failed.(Global Data)", L"Constant Buffer creation failed.(Global Data)", MB_OK);
        return E_FAIL;
    }
    m_arrCB[(UINT)CB_TYPE::GLOBAL] = pCB;

    return S_OK;
}

int DDevice::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC Desc = {};

    // Cull Back
    m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

    // Cull Front 
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

    // Cull None
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

    // Wire Frame
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

    return S_OK;
}

int DDevice::CreateDepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC Desc = {};

    // Less
    m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

    // Less Equal
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf())))
    {
        return E_FAIL;
    }

    // Greater
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_GREATER;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf())))
    {
        return E_FAIL;
    }

    // NO_TEST
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST].GetAddressOf())))
    {
        return E_FAIL;
    }

    // NO_WRITE
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_WRITE].GetAddressOf())))
    {
        return E_FAIL;
    }

    // NO_TEST_NO_WRITE
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf())))
    {
        return E_FAIL;
    }


    return S_OK;
}

int DDevice::CreateBlendState()
{
    D3D11_BLEND_DESC Desc = {};

    // Default
    m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;

    // AlphaBlend - Coverage
    Desc.AlphaToCoverageEnable = true;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Src(Pixel RGB) * A   +    Dest(RenderTarget RGB) * (1 - A)
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND_COVERAGE].GetAddressOf())))
    {
        return E_FAIL;
    }

    // AlphaBlend
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Src(Pixel RGB) * A   +   Dest(RenderTarget RGB) * (1 - A)
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf())))
    {
        return E_FAIL;
    }

    // One - One Blend
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Src(Pixel RGB) * 1   +   Dest(RenderTarget RGB) * 1
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ONE_ONE].GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int DDevice::CreateSamplerState()
{
    D3D11_SAMPLER_DESC Desc = {};

    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_ANISOTROPIC;

    if (FAILED(DEVICE->CreateSamplerState(&Desc, m_Sampler[0].GetAddressOf())))
    {
        return E_FAIL;
    }

    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

    if (FAILED(DEVICE->CreateSamplerState(&Desc, m_Sampler[1].GetAddressOf())))
    {
        return E_FAIL;
    }

    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    if (FAILED(DEVICE->CreateSamplerState(&Desc, m_Sampler[2].GetAddressOf())))
    {
        return E_FAIL;
    }

    CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

    CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->CSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());

    CONTEXT->VSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->HSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->DSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->GSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->PSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->CSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());

    return S_OK;
}
