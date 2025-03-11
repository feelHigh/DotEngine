#include "pch.h"
#include "DTaskMgr.h"

// Manager Headers
#include "DAssetMgr.h"
#include "DLevelMgr.h"

// Level Headers
#include "DLevel.h"
#include "DLayer.h"

// GameObject Headers
#include "DGameObject.h"

DTaskMgr::DTaskMgr()
{
}

DTaskMgr::~DTaskMgr()
{
}

void DTaskMgr::Tick()
{
	ClearGC();

	ExecuteTask();
}

void DTaskMgr::AddTask(const tTask& _Task)
{
	m_vecTask.push_back(_Task);
}

void DTaskMgr::ClearGC()
{
	Delete_Vec(m_GC);
}

void DTaskMgr::ExecuteTask()
{
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		tTask& task = m_vecTask[i];
		switch (task.Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			DLevel* pCurLevel = DLevelMgr::GetInst()->GetCurrentLevel();
			int LayerIndex = (int)task.Param_0;
			DGameObject* pObject = (DGameObject*)task.Param_1;
			pCurLevel->AddObject(LayerIndex, pObject);

			if (pCurLevel->GetState() != STOP)
			{
				pObject->Begin();
			}
		}
		break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			DGameObject* pObject = (DGameObject*)task.Param_0;
			if (pObject->m_Dead)
			{
				continue;
			}

			// Garbage Collector
			pObject->m_Dead = true;
			m_GC.push_back(pObject);
		}
		break;

		case TASK_TYPE::CHANGE_LEVELSTATE:
		{
			LEVEL_STATE NextState = (LEVEL_STATE)task.Param_0;
			DLevel* pCurLevel = DLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->ChangeState(NextState);
		}
		break;

		case TASK_TYPE::CHANGE_LEVEL:
		{
			// Param_0 : Level Adress, Param_1 : Level State
			DLevel* pLevel = (DLevel*)task.Param_0;
			LEVEL_STATE NextState = (LEVEL_STATE)task.Param_1;

			DLevelMgr::GetInst()->ChangeLevel(pLevel);
			pLevel->ChangeState(NextState);
		}
		break;

		case TASK_TYPE::ASSET_CHANGED:
		{
			DAssetMgr::GetInst()->m_Changed = true;
		}

		case TASK_TYPE::LEVEL_CHANGED:
		{
			DLevelMgr::GetInst()->m_LevelChanged = true;
		}
		break;

		case TASK_TYPE::DEL_ASSET:
		{
			DAsset* pAsset = (DAsset*)task.Param_0;
			ASSET_TYPE Type = pAsset->GetAssetType();
			DAssetMgr::GetInst()->DeleteAsset(Type, pAsset->GetKey());
		}
		break;
		}
	}

	m_vecTask.clear();
}