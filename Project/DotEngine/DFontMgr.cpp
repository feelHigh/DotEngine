#include "pch.h"
#include "DFontMgr.h"

// Device Headers
#include "DDevice.h"

DFontMgr::DFontMgr()
	: m_FW1Factory(nullptr)
	, m_FontWrapper(nullptr)
{
}

DFontMgr::~DFontMgr()
{
	if (nullptr != m_FW1Factory)
		m_FW1Factory->Release();

	if (nullptr != m_FontWrapper)
		m_FontWrapper->Release();
}

void DFontMgr::Init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(nullptr);
	}

	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_FontWrapper)))
	{
		assert(nullptr);
	}
}

void DFontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_FontWrapper->DrawString(CONTEXT,
							_pStr,				// String
							_fFontSize,			// Font size
							_fPosX,				// X position
							_fPosY,				// Y position
							_Color,				// Text color, 0xAaBbGgRr
							FW1_RESTORESTATE    // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}
