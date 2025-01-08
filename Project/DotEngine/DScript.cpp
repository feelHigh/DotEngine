#include "pch.h"
#include "DScript.h"

DScript::DScript(UINT _ScriptType)
	: DComponent(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_ScriptType)
{
}

DScript::~DScript()
{
}

void DScript::Instantiate(Ptr<DPrefab> _Pref, int _LayerIdx, Vec3 _WorldPos, const wstring& _Name)
{
	DGameObject* pInst = _Pref->Instantiate();

	pInst->SetName(_Name);
	pInst->Transform()->SetRelativePos(_WorldPos);

	CreateObject(pInst, _LayerIdx);
}
