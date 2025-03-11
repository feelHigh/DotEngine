#ifndef ENGINE_DPATHMGR_H
#define ENGINE_DPATHMGR_H

#include "Singleton.h"

class DPathMgr
	: public DSingleton<DPathMgr>
{
	SINGLE(DPathMgr);
public:
	void Init();

public:
	const wstring& GetContentPath() { return m_Content; }
	const wstring& GetSolutionPath() { return m_Solution; }
	wstring GetRelativePath(const wstring& _FilePath);

private:
	void GetParentPath(_Inout_ wchar_t* _Buffer);

private:
	wstring		m_Content;
	wstring		m_Solution;
};

#endif // !ENGINE_DPATHMGR_H