#include "pch.h"
#include "DLevelMgr.h"

// Manager Headers
#include "DTaskMgr.h"

// Level Headers
#include "DLevel.h"
#include "DLayer.h"

// GameObject Headers
#include "DGameObject.h"

DLevelMgr::DLevelMgr()
	: m_CurLevel(nullptr)
{

}

DLevelMgr::~DLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

DGameObject* DLevelMgr::FindObjectByName(const wstring& _Name)
{
	if (m_CurLevel)
	{
		return m_CurLevel->FindObjectByName(_Name);
	}

	return nullptr;
}

void DLevelMgr::LevelChanged()
{
	DTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::LEVEL_CHANGED });
}

void DLevelMgr::Init()
{
}

void DLevelMgr::Progress()
{
	m_LevelChanged = false;
	if (nullptr == m_CurLevel)
		return;

	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		m_CurLevel->Tick();
	}

	m_CurLevel->ClearObject();
	m_CurLevel->FinalTick();
}

bool DLevelMgr::ChangeLevel(DLevel* _NextLevel)
{
	if (m_CurLevel == _NextLevel)
		return false;

	if (nullptr != m_CurLevel)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	// If level has been changed -> event triggered
	LevelChanged();

	return true;
}
