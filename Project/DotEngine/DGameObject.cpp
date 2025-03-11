#include "pch.h"
#include "DGameObject.h"

// Base Headers
#include "ComponentList.h"

// Manager Headers
#include "DTimeMgr.h"
#include "DKeyMgr.h"
#include "DLevelMgr.h"

// Level Headers
#include "DLevel.h"
#include "DLayer.h"

// Component Headers
#include "DScript.h"

DGameObject::DGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1) // Not belonging to any layer (not in level)
	, m_Dead(false)
{
}

DGameObject::DGameObject(const DGameObject& _Origin)
	: DEntity(_Origin)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)
{
	// Copy Components
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _Origin.m_arrCom[i])
			continue;

		DComponent* pClonedCom = _Origin.m_arrCom[i]->Clone();

		AddComponent(pClonedCom);

		pClonedCom->Init();
	}

	// Copy Script
	for (size_t i = 0; i < _Origin.m_vecScript.size(); ++i)
	{
		AddComponent(_Origin.m_vecScript[i]->Clone());
	}

	// Copy child object
	for (size_t i = 0; i < _Origin.m_vecChildren.size(); ++i)
	{
		AddChild(_Origin.m_vecChildren[i]->Clone());
	}
}

DGameObject::~DGameObject()
{
	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChildren);
}

void DGameObject::AddComponent(DComponent* _Comopnent)
{
	COMPONENT_TYPE Type = _Comopnent->GetComponentType();

	if (COMPONENT_TYPE::SCRIPT == Type)
	{
		m_vecScript.push_back((DScript*)_Comopnent);
		_Comopnent->SetOwner(this);
	}
	else
	{
		assert(nullptr == m_arrCom[(UINT)Type]);

		m_arrCom[(UINT)Type] = _Comopnent;
		m_arrCom[(UINT)Type]->SetOwner(this);

		DRenderComponent* pRenderCom = dynamic_cast<DRenderComponent*>(_Comopnent);

		assert(!(pRenderCom && m_RenderCom));

		if (pRenderCom)
		{
			m_RenderCom = pRenderCom;
		}
	}
}

void DGameObject::AddChild(DGameObject* _ChildObject)
{
	// If the parent object belongs to a Level and the child object to be added does not belong to a Level
	if (-1 != m_LayerIdx && -1 == _ChildObject->m_LayerIdx)
	{
		assert(nullptr);
	}

	// The object entering the child already has a parent
	if (_ChildObject->GetParent())
	{
		_ChildObject->DeregisterChild();
	}

	// The object entering the child is the parent object at the top
	else
	{
		if (-1 != _ChildObject->m_LayerIdx)
		{
			DLevel* pCurLevel = DLevelMgr::GetInst()->GetCurrentLevel();
			if (nullptr != pCurLevel)
			{
				DLayer* pLayer = pCurLevel->GetLayer(_ChildObject->m_LayerIdx);
				pLayer->DeregisterObjectAsParent(_ChildObject);
			}
		}
	}

	m_vecChildren.push_back(_ChildObject);
	_ChildObject->m_Parent = this;

	DLevelMgr::GetInst()->LevelChanged();
}

bool DGameObject::IsAncestor(DGameObject* _Object)
{
	DGameObject* pObject = m_Parent;

	while (pObject)
	{
		if (pObject == _Object)
			return true;
		else
			pObject = pObject->GetParent();
	}

	return false;
}

void DGameObject::DisconnectWithLayer()
{
	if (nullptr == m_Parent)
	{
		DLevel* pLevel = DLevelMgr::GetInst()->GetCurrentLevel();
		DLayer* pLayer = pLevel->GetLayer(m_LayerIdx);
		pLayer->DisconnectWithObject(this);
	}

	m_LayerIdx = -1;
}

void DGameObject::DeregisterChild()
{
	DLevelMgr::GetInst()->LevelChanged();

	vector<DGameObject*>::iterator iter = m_Parent->m_vecChildren.begin();

	for (; iter != m_Parent->m_vecChildren.end(); ++iter)
	{
		if ((*iter) == this)
		{
			m_Parent->m_vecChildren.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	assert(nullptr);
}

void DGameObject::Begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrCom[i])
			continue;

		m_arrCom[i]->Begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Begin();
	}

	// Child Object
	for (size_t i = 0; i < m_vecChildren.size(); ++i)
	{
		m_vecChildren[i]->Begin();
	}
}

void DGameObject::Tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->Tick();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Tick();
	}

	// Child Object
	for (size_t i = 0; i < m_vecChildren.size(); ++i)
	{
		m_vecChildren[i]->Tick();
	}
}

void DGameObject::FinalTick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->FinalTick();
	}

	// Register Layer
	assert(-1 != m_LayerIdx);
	DLevel* pLevel = DLevelMgr::GetInst()->GetCurrentLevel();
	DLayer* pLayer = pLevel->GetLayer(m_LayerIdx);
	pLayer->RegisterGameObject(this);

	// Child Object
	vector<DGameObject*>::iterator iter = m_vecChildren.begin();
	for (; iter != m_vecChildren.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChildren.erase(iter);
		else
			++iter;
	}
}

void DGameObject::Render()
{
	if (m_RenderCom)
		m_RenderCom->Render();
}
