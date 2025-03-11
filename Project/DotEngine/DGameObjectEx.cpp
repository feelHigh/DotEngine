#include "pch.h"
#include "DGameObjectEx.h"

// Base headers
#include "ComponentList.h"

void DGameObjectEx::FinalTick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != GetComponent((COMPONENT_TYPE)i))
			GetComponent((COMPONENT_TYPE)i)->FinalTick();
	}

	// Child object
	vector<DGameObject*>::const_iterator iter = GetChildren().begin();
	for (; iter != GetChildren().end(); ++iter)
	{
		(*iter)->FinalTick();
	}
}
