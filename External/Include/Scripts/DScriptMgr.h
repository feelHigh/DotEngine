#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	CAMERAMOVESCRIPT,
	EDITORCAMERASCRIPT,
	MISSILESCRIPT,
	PLAYERSCRIPT,
};

using namespace std;

class DScript;

class DScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static DScript * GetScript(const wstring& _strScriptName);
	static DScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(DScript * _pScript);
};
