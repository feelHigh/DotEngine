#include "pch.h"
#include "DLayer.h"

// GameObject Headers
#include "DGameObject.h"

DLayer::DLayer(int _LayerIdx)
	: m_LayerIdx(_LayerIdx)
{
}

DLayer::DLayer(const DLayer& _Origin)
	: DEntity(_Origin)
	, m_LayerIdx(_Origin.m_LayerIdx)
{
	for (size_t i = 0; i < _Origin.m_Parents.size(); ++i)
	{
		AddObject(_Origin.m_Parents[i]->Clone(), false);
	}
}

DLayer::~DLayer()
{
	Delete_Vec(m_Parents);
}

void DLayer::Begin()
{
	for (size_t i = 0; i < m_Parents.size(); ++i)
	{
		m_Parents[i]->Begin();
	}
}

void DLayer::Tick()
{
	for (size_t i = 0; i < m_Parents.size(); ++i)
	{
		m_Parents[i]->Tick();
	}
}

void DLayer::FinalTick()
{
	vector<DGameObject*>::iterator iter = m_Parents.begin();

	for (; iter != m_Parents.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_Parents.erase(iter);
		else
			++iter;
	}
}

void DLayer::AddObject(DGameObject* _Object, bool _bChildMove)
{
	// 1. 오브젝트가 다른 레이어 소속인경우
	if (-1 != _Object->GetLayerIdx())
	{
		assert(_Object->GetLayerIdx() != m_LayerIdx);

		// 기존에 소속된 레이어에서 빠져나와야한다.
		_Object->DisconnectWithLayer();
	}

	// 2. 최상위 오브젝트인 경우
	if (nullptr == _Object->GetParent())
	{
		m_Parents.push_back(_Object);
	}

	// 자식들까지 이동시킬지 말지
	static list<DGameObject*> queue;
	queue.clear();
	queue.push_back(_Object);
	_Object->m_LayerIdx = m_LayerIdx;

	while (!queue.empty())
	{
		DGameObject* pObject = queue.front();
		queue.pop_front();

		if (_bChildMove || pObject->m_LayerIdx == -1)
		{
			pObject->m_LayerIdx = m_LayerIdx;
		}

		const vector<DGameObject*>& vecChildren = pObject->GetChildren();
		for (size_t i = 0; i < vecChildren.size(); ++i)
		{
			queue.push_back(vecChildren[i]);
		}
	}
}

void DLayer::DisconnectWithObject(DGameObject* _Object)
{
	vector<DGameObject*>::iterator iter = m_Parents.begin();
	for (; iter != m_Parents.end(); ++iter)
	{
		if (_Object == (*iter))
		{
			m_Parents.erase(iter);
			return;
		}
	}

	assert(nullptr);
}

void DLayer::RegisterAsParent(DGameObject* _Object)
{
	assert(_Object->GetLayerIdx() == m_LayerIdx && !_Object->GetParent());

	m_Parents.push_back(_Object);
	return;
}

void DLayer::DeregisterObjectAsParent(DGameObject* _Object)
{
	vector<DGameObject*>::iterator iter = m_Parents.begin();

	for (; iter != m_Parents.end(); ++iter)
	{
		if (_Object == (*iter))
		{
			m_Parents.erase(iter);
			return;
		}
	}

	assert(nullptr);
}

void DLayer::DeregisterObject(DGameObject* _Object)
{
	assert(_Object->m_LayerIdx == m_LayerIdx);

	_Object->m_LayerIdx = -1;

	if (nullptr == _Object->GetParent())
	{
		vector<DGameObject*>::iterator iter = m_Parents.begin();

		for (; iter != m_Parents.end(); ++iter)
		{
			if (_Object == (*iter))
			{
				m_Parents.erase(iter);
				return;
			}
		}

		assert(nullptr);
	}
}
