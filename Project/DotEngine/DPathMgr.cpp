#include "pch.h"
#include "DPathMgr.h"

// Engine Headers
#include "DEngine.h"

DPathMgr::DPathMgr()
{
}

DPathMgr::~DPathMgr()
{
}

void DPathMgr::Init()
{
	wchar_t szBuffer[256] = {};
	GetCurrentDirectory(256, szBuffer);

	// Access the parent folder of the bin folder
	GetParentPath(szBuffer);
	GetParentPath(szBuffer);

	// Content path
	m_Content = szBuffer;
	m_Content += L"\\Content\\";

	// Solution path
	m_Solution = szBuffer;
}

void DPathMgr::GetParentPath(_Inout_ wchar_t* _Buffer)
{
	size_t len = wcslen(_Buffer);

	for (size_t i = len - 1; 0 < i; --i)
	{
		if (L'\\' == _Buffer[i])
		{
			_Buffer[i] = L'\0';
			break;
		}
	}
}

wstring DPathMgr::GetRelativePath(const wstring& _FilePath)
{
	size_t FindPos = _FilePath.find(m_Content);
	if (FindPos == wstring::npos)
		return L"";

	return _FilePath.substr(FindPos + m_Content.length(), _FilePath.length());
}
