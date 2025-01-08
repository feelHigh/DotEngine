#include "pch.h"
#include "DScriptMgr.h"

#include "CameraMoveScript.h"
#include "EditorCameraScript.h"
#include "MissileScript.h"
#include "PlayerScript.h"

void DScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CameraMoveScript");
	_vec.push_back(L"EditorCameraScript");
	_vec.push_back(L"MissileScript");
	_vec.push_back(L"PlayerScript");
}

DScript * DScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CameraMoveScript" == _strScriptName)
		return new CameraMoveScript;
	if (L"EditorCameraScript" == _strScriptName)
		return new EditorCameraScript;
	if (L"MissileScript" == _strScriptName)
		return new MissileScript;
	if (L"PlayerScript" == _strScriptName)
		return new PlayerScript;
	return nullptr;
}

DScript * DScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::EDITORCAMERASCRIPT:
		return new EditorCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new MissileScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new PlayerScript;
		break;
	}
	return nullptr;
}

const wchar_t * DScriptMgr::GetScriptName(DScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CameraMoveScript";
		break;

	case SCRIPT_TYPE::EDITORCAMERASCRIPT:
		return L"EditorCameraScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"MissileScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"PlayerScript";
		break;

	}
	return nullptr;
}