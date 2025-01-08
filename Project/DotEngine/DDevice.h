#pragma once
// DirectX ���� ����� ���
// GPU ����
class DConstantBuffer;

#include "DTexture.h"

class DDevice
	: public DSingleton<DDevice>
{
	SINGLE(DDevice);
public:
	int Init(HWND _hWnd, UINT _Width, UINT _Height);
	void Present() { m_SwapChain->Present(0, 0); }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateConstBuffer();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();

public:
	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }

	DConstantBuffer* GetConstBuffer(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }
	ID3D11RasterizerState* GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type].Get(); }
	ID3D11DepthStencilState* GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type].Get(); }
	ID3D11BlendState* GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type].Get(); }

public:
	Vec2 GetResolution() { return m_vResolution; }

private:
	HWND							m_hWnd;
	Vec2							m_vResolution;

	ComPtr<ID3D11Device>			m_Device;	// Dx11 ��ü ����, GPU �޸� �Ҵ�
	ComPtr<ID3D11DeviceContext>		m_Context;	// Rendering, 

	ComPtr<IDXGISwapChain>			m_SwapChain;

	Ptr<DTexture>					m_RTTex;
	Ptr<DTexture>					m_DSTex;

	ComPtr<ID3D11RasterizerState>	m_RSState[(UINT)RS_TYPE::END];
	ComPtr<ID3D11SamplerState>		m_Sampler[3];

	ComPtr<ID3D11DepthStencilState>	m_DSState[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_BSState[(UINT)BS_TYPE::END];

	DConstantBuffer*				m_arrCB[(UINT)CB_TYPE::END];

};
