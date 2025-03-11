#include "pch.h"
#include "DTexture.h"

// Device Headers
#include "DDevice.h"

DTexture::DTexture()
	: DAsset(ASSET_TYPE::TEXTURE)
	, m_Desc{}
{
}

DTexture::~DTexture()
{
}

void DTexture::Binding(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void DTexture::Clear(UINT _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}

void DTexture::Binding_CS_SRV(UINT _RegisterNum)
{
	m_RecentBindingRegisterNum = _RegisterNum;
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void DTexture::Clear_CS_SRV()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(m_RecentBindingRegisterNum, 1, &pSRV);
}

void DTexture::Binding_CS_UAV(UINT _RegisterNum)
{
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
	m_RecentBindingRegisterNum = _RegisterNum;
}

void DTexture::Clear_CS_UAV()
{
	UINT i = -1;
	ID3D11UnorderedAccessView* pUAV = nullptr;
	CONTEXT->CSSetUnorderedAccessViews(m_RecentBindingRegisterNum, 1, &pUAV, &i);
}

int DTexture::Load(const wstring& _FilePath)
{
	path filepath = _FilePath;
	path extention = filepath.extension();

	HRESULT hr = S_OK;

	// *.dds
	if (extention == L".dds" || extention == L".DDS")
	{
		hr = LoadFromDDSFile(_FilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
	}

	// *.tga
	else if (extention == L".tga" || extention == L".TGA")
	{
		hr = LoadFromTGAFile(_FilePath.c_str(), nullptr, m_Image);
	}

	// Window Image Component (*.bmp, *.png, *.jpg, *.jpeg )
	else if (extention == L".bmp" || extention == L".png" || extention == L".jpg" || extention == L".jpeg"
		|| extention == L".BMP" || extention == L".PNG" || extention == L".JPG" || extention == L".JPEG")
	{
		hr = LoadFromWICFile(_FilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	// Unknown format
	else
	{
		return E_FAIL;
	}

	// If loading failed
	if (FAILED(hr))
	{
		errno_t ErrNum = GetLastError();
		wchar_t szBuff[255] = {};
		wsprintf(szBuff, L"Error Number : %d", ErrNum);
		MessageBox(nullptr, szBuff, L"Failed loading texture.(DTexture)", MB_OK);
		return E_FAIL;
	}

	// SystemMem(ScratchImage) -> GPU(Texture2D)
	CreateShaderResourceView(DEVICE
							, m_Image.GetImages()
							, m_Image.GetImageCount()
							, m_Image.GetMetadata()
							, m_SRV.GetAddressOf());

	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	m_Tex2D->GetDesc(&m_Desc);

	return S_OK;
}

int DTexture::Save(const wstring& _FilePath)
{
	OutputDebugString(L"=== Starting DTexture::Save ===\n");
	OutputDebugString((L"Path: " + _FilePath + L"\n").c_str());

	// Get the resource from our SRV
	ComPtr<ID3D11Resource> resource;
	if (m_SRV)
	{
		m_SRV->GetResource(resource.GetAddressOf());
	}
	else
	{
		// Use the texture directly if SRV is not available
		resource = m_Tex2D;
	}

	if (!resource)
	{
		OutputDebugString(L"Error: No resource available for saving\n");
		return E_FAIL;
	}

	// Cast to ID3D11Texture2D
	ComPtr<ID3D11Texture2D> texture2D;
	HRESULT hr = resource.As(&texture2D);
	if (FAILED(hr))
	{
		OutputDebugString(L"Error: Failed to cast resource to ID3D11Texture2D\n");
		return E_FAIL;
	}

	// Get the texture description
	D3D11_TEXTURE2D_DESC texDesc;
	texture2D->GetDesc(&texDesc);

	// If this is a render target or has no CPU access, we need to create a staging texture
	if ((texDesc.Usage != D3D11_USAGE_STAGING) || (texDesc.CPUAccessFlags & D3D11_CPU_ACCESS_READ) != D3D11_CPU_ACCESS_READ)
	{
		OutputDebugString(L"Creating staging texture for CPU access\n");

		// Create a staging texture
		D3D11_TEXTURE2D_DESC stagingDesc = texDesc;
		stagingDesc.BindFlags = 0;
		stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagingDesc.Usage = D3D11_USAGE_STAGING;

		ComPtr<ID3D11Texture2D> stagingTexture;
		hr = DEVICE->CreateTexture2D(&stagingDesc, nullptr, stagingTexture.GetAddressOf());
		if (FAILED(hr))
		{
			OutputDebugString(L"Error: Failed to create staging texture\n");
			return E_FAIL;
		}

		// Copy the original texture to the staging texture
		CONTEXT->CopyResource(stagingTexture.Get(), texture2D.Get());

		// Use the staging texture instead
		texture2D = stagingTexture;
	}

	// Create scratch image and capture the texture
	DirectX::ScratchImage image;
	hr = DirectX::CaptureTexture(DEVICE, CONTEXT, texture2D.Get(), image);
	if (FAILED(hr))
	{
		wchar_t errMsg[256];
		swprintf_s(errMsg, L"Error: Failed to capture texture. HRESULT: 0x%08X\n", hr);
		OutputDebugString(errMsg);
		return E_FAIL;
	}

	// Determine the file format from extension
	wstring ext = _FilePath.substr(_FilePath.find_last_of(L"."));
	std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);

	// Save based on file extension
	if (ext == L".png")
	{
		OutputDebugString(L"Saving as PNG...\n");
		hr = DirectX::SaveToWICFile(*image.GetImage(0, 0, 0), DirectX::WIC_FLAGS_NONE,
			GetWICCodec(DirectX::WIC_CODEC_PNG), _FilePath.c_str());
	}
	else if (ext == L".jpg" || ext == L".jpeg")
	{
		OutputDebugString(L"Saving as JPEG...\n");
		hr = DirectX::SaveToWICFile(*image.GetImage(0, 0, 0), DirectX::WIC_FLAGS_NONE,
			GetWICCodec(DirectX::WIC_CODEC_JPEG), _FilePath.c_str());
	}
	else if (ext == L".bmp")
	{
		OutputDebugString(L"Saving as BMP...\n");
		hr = DirectX::SaveToWICFile(*image.GetImage(0, 0, 0), DirectX::WIC_FLAGS_NONE,
			GetWICCodec(DirectX::WIC_CODEC_BMP), _FilePath.c_str());
	}
	else if (ext == L".tga")
	{
		OutputDebugString(L"Saving as TGA...\n");
		hr = DirectX::SaveToTGAFile(*image.GetImage(0, 0, 0), _FilePath.c_str());
	}
	else if (ext == L".dds")
	{
		OutputDebugString(L"Saving as DDS...\n");
		hr = DirectX::SaveToDDSFile(image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			DirectX::DDS_FLAGS_NONE, _FilePath.c_str());
	}
	else
	{
		// Default to PNG if extension not recognized
		OutputDebugString(L"Unknown extension, defaulting to PNG...\n");
		hr = DirectX::SaveToWICFile(*image.GetImage(0, 0, 0), DirectX::WIC_FLAGS_NONE,
			GetWICCodec(DirectX::WIC_CODEC_PNG), _FilePath.c_str());
	}

	if (SUCCEEDED(hr))
	{
		OutputDebugString(L"Save succeeded\n");
	}
	else
	{
		wchar_t errMsg[256];
		swprintf_s(errMsg, L"Save failed with HRESULT: 0x%08X\n", hr);
		OutputDebugString(errMsg);
	}

	OutputDebugString(L"=== Finished DTexture::Save ===\n");
	return hr;
}



// RENDER_TARGET | SHADER_RESOURCE
// DEPTH_STENCIL
// SHADER_RESOURCE | UNORDERED_ACESS
int DTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _Flags, D3D11_USAGE _Usage)
{
	m_Desc.Width = _Width; // The DepthStencil texture must match the render target resolution
	m_Desc.Height = _Height;
	m_Desc.Format = _PixelFormat; // Depth 24bit , Stencil 8bit
	m_Desc.ArraySize = 1;
	m_Desc.BindFlags = _Flags;

	m_Desc.Usage = _Usage;   // Establish associations with System Memory

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_Desc.CPUAccessFlags = 0;
	}

	m_Desc.MiscFlags = 0;
	m_Desc.MipLevels = 1;   // Create Additional Degradation Version Resolution Images

	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}

	// Create view
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());
	}

	return S_OK;
}

int DTexture::Create(ComPtr<ID3D11Texture2D> _Tex2D)
{
	m_Tex2D = _Tex2D;
	m_Tex2D->GetDesc(&m_Desc);

	// Create view
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());
	}

	return S_OK;
}
