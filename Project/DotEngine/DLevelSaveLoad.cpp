#include "pch.h"
#include "DLevelSaveLoad.h"

// Base Headers
#include "ComponentList.h"

// Manager Headers
#include "DLevelMgr.h"

// Level Headers
#include "DLevel.h"
#include "DLayer.h"

// GameObject Headers
#include "DGameObject.h"

// Scripts
#include <Scripts\DScriptMgr.h>

void DLevelSaveLoad::SaveLevel(const wstring& _FilePath, DLevel* _Level)
{
	assert(_Level && _Level->GetState() == LEVEL_STATE::STOP);

	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"wb");

	// Save Level Name
	SaveWString(_Level->GetName(), File);

	// Save Layer in Level
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		DLayer* pLayer = _Level->GetLayer(i);

		// Save Layer Name
		SaveWString(pLayer->GetName(), File);

		// Save GameObject in Layer
		const vector<DGameObject*>& vecParents = pLayer->GetParentObjects();

		// Save the number of ParentObjects held by Layer
		size_t count = vecParents.size();
		fwrite(&count, sizeof(size_t), 1, File);

		// Save GameObject
		for (size_t i = 0; i < vecParents.size(); ++i)
		{
			SaveGameObject(File, vecParents[i]);
		}
	}

	fclose(File);
}

void DLevelSaveLoad::SaveGameObject(FILE* _File, DGameObject* _Object)
{
	// Save GameObject Name
	SaveWString(_Object->GetName(), _File);

	// Save Component Information
	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		DComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);

		if (nullptr == pComponent)
			continue;

		// Save ComponentType
		COMPONENT_TYPE Type = pComponent->GetComponentType();
		fwrite(&Type, sizeof(COMPONENT_TYPE), 1, _File);

		pComponent->SaveToFile(_File);
	}

	// COMPONENT_TYPE::END
	fwrite(&i, sizeof(COMPONENT_TYPE), 1, _File);

	// Save Script Information
	// Save Script count
	const vector<DScript*> vecScripts = _Object->GetScripts();
	size_t ScriptCount = vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		// Save Script Name
		wstring ScriptName = DScriptMgr::GetScriptName(vecScripts[i]);
		SaveWString(ScriptName, _File);

		// Save Script Information
		vecScripts[i]->SaveToFile(_File);
	}

	// Save Child Information
	const vector<DGameObject*>& vecChild = _Object->GetChildren();
	size_t ChildCount = vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		SaveGameObject(_File, vecChild[i]);
	}
}

DLevel* DLevelSaveLoad::LoadLevel(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"rb");

	// Create Level
	DLevel* pNewLevel = new DLevel;

	// Import Level Name
	wstring LevelName;
	LoadWString(LevelName, File);
	pNewLevel->SetName(LevelName);

	// Import Layer information within Level
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		DLayer* pLayer = pNewLevel->GetLayer(i);

		// Import Layer
		wstring LayerName;
		LoadWString(LayerName, File);
		pLayer->SetName(LayerName);

		// Import the number of ParentObjects held by Layer
		size_t count = 0;
		fread(&count, sizeof(size_t), 1, File);

		// Call GameObject and add to Layer
		for (size_t i = 0; i < count; ++i)
		{
			DGameObject* pLoadedObject = LoadGameObject(File);
			pLayer->AddObject(pLoadedObject, false);
		}
	}

	fclose(File);

	return pNewLevel;
}

DGameObject* DLevelSaveLoad::LoadGameObject(FILE* _File)
{
	DGameObject* pObject = new DGameObject;

	// Load GameObject Name
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// Load Component Information	
	COMPONENT_TYPE Type = COMPONENT_TYPE::END;
	while (true)
	{
		// Determine which component the stored information is
		fread(&Type, sizeof(COMPONENT_TYPE), 1, _File);

		// If the read type information is END, it is the end of the stored component information
		if (COMPONENT_TYPE::END == Type)
			break;

		// Create components that fit the stored type and read the data in the same order as when storing them
		DComponent* pComponent = GetComponent(Type);

		// Put the created component into the object
		pObject->AddComponent(pComponent);

		// Read and copy the information at the time of storage
		pComponent->LoadFromFile(_File);
	}

	// Load Script Information
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		// Read Script name
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// Create script corresponding to the name
		DScript* pScript = DScriptMgr::GetScript(ScriptName);
		pScript->LoadFromFile(_File);

		pObject->AddComponent(pScript);
	}

	// Load Child Information
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		DGameObject* pChildObject = LoadGameObject(_File);
		pObject->AddChild(pChildObject);
	}

	return pObject;
}

DComponent* DLevelSaveLoad::GetComponent(COMPONENT_TYPE _Type)
{
	switch (_Type)
	{
	case COMPONENT_TYPE::TRANSFORM:
		return new DTransform;

	case COMPONENT_TYPE::COLLIDER2D:
		return new DCollider2D;

	case COMPONENT_TYPE::PAPERFLIPBOOK:
		return new DPaperFlipbook;

	case COMPONENT_TYPE::LIGHT2D:
		return new DLight2D;

	case COMPONENT_TYPE::CAMERA:
		return new DCamera;

	case COMPONENT_TYPE::MESHRENDER:
		return new DMeshRender;

	case COMPONENT_TYPE::TILEMAP:
		return new DTileMap;

	case COMPONENT_TYPE::PARTICLESYSTEM:
		return new DParticleSystem;

	}

	return nullptr;
}
