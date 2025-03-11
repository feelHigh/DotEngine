#include "pch.h"
#include "DComputeShader.h"

// Device Headers
#include "DDevice.h"
#include "DConstantBuffer.h"

// Manager Headers
#include "DPathMgr.h"

DComputeShader::DComputeShader(UINT _ThreadPerGroupX, UINT _ThreadPerGroupY, UINT _ThreadPerGroupZ
	, const wstring& _ShaderRelativePath, const string& _FuncName)
	: DShader(ASSET_TYPE::COMPUTE_SHADER)
	, m_ThreadPerGroupX(_ThreadPerGroupX)
	, m_ThreadPerGroupY(_ThreadPerGroupY)
	, m_ThreadPerGroupZ(_ThreadPerGroupZ)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
	, m_Const{}
{
	CreateComputeShader(_ShaderRelativePath, _FuncName);
}

DComputeShader::~DComputeShader()
{
}


int DComputeShader::Execute()
{
	if (FAILED(Binding()))
		return E_FAIL;

	// Pass on constant buffer
	DConstantBuffer* pCB = DDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Binding_CS();

	// Calculate needed group count
	CalcGroupNum();

	// Execute compute shader
	CONTEXT->CSSetShader(m_CS.Get(), 0, 0);
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	// Clear resource
	Clear();
}

int DComputeShader::CreateComputeShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = DPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
									, _FuncName.c_str(), "cs_5_0", D3DCOMPILE_DEBUG, 0
									, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != m_ErrBlob)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Shader Compile failed.(Compute Shader)", MB_OK);
		}
		else
		{
			errno_t err = GetLastError();
			wchar_t szErrMsg[255] = {};
			swprintf_s(szErrMsg, 255, L"Error Code : %d", err);
			MessageBox(nullptr, szErrMsg, L"Shader Compile failed.(Compute Shader)", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize(), nullptr, m_CS.GetAddressOf());

	return S_OK;
}
