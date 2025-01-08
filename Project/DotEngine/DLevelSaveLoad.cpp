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

	// Level �̸� ����
	SaveWString(_Level->GetName(), File);

	// Level �ȿ� �ִ� Layer �� ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		DLayer* pLayer = _Level->GetLayer(i);

		// Layer �̸� ����
		SaveWString(pLayer->GetName(), File);

		// Layer �ȿ� �ִ� GameObject �� ����
		const vector<DGameObject*>& vecParents = pLayer->GetParentObjects();

		// Layer �� ������ ParentObject ���� ����
		size_t count = vecParents.size();
		fwrite(&count, sizeof(size_t), 1, File);

		// GameObject ����
		for (size_t i = 0; i < vecParents.size(); ++i)
		{
			SaveGameObject(File, vecParents[i]);
		}
	}

	fclose(File);
}

void DLevelSaveLoad::SaveGameObject(FILE* _File, DGameObject* _Object)
{
	// GameObject �� �̸� ����
	SaveWString(_Object->GetName(), _File);

	// Component ���� ����
	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		DComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);

		if (nullptr == pComponent)
			continue;

		// ComponentType �� ����
		COMPONENT_TYPE Type = pComponent->GetComponentType();
		fwrite(&Type, sizeof(COMPONENT_TYPE), 1, _File);

		pComponent->SaveToFile(_File);
	}

	// COMPONENT_TYPE::END ����
	fwrite(&i, sizeof(COMPONENT_TYPE), 1, _File);

	// Script ���� ����
	// Script ���� ����
	const vector<DScript*> vecScripts = _Object->GetScripts();
	size_t ScriptCount = vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		// Script �� �̸� ����
		wstring ScriptName = DScriptMgr::GetScriptName(vecScripts[i]);
		SaveWString(ScriptName, _File);

		// Script ���� ����
		vecScripts[i]->SaveToFile(_File);
	}

	// Child ���� ����
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

	// Level ����
	DLevel* pNewLevel = new DLevel;

	// Level �̸� �ҷ�����
	wstring LevelName;
	LoadWString(LevelName, File);
	pNewLevel->SetName(LevelName);

	// Level �ȿ� �ִ� Layer ���� �ҷ�����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		DLayer* pLayer = pNewLevel->GetLayer(i);

		// Layer �̸� �ҷ�����
		wstring LayerName;
		LoadWString(LayerName, File);
		pLayer->SetName(LayerName);

		// Layer �� ������ ParentObject ���� �Ҿ����
		size_t count = 0;
		fread(&count, sizeof(size_t), 1, File);

		// GameObject �ҷ��ͼ� Layer �� ����ֱ�
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

	// GameObject �� �̸� �ε�
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// Component ���� �ε�	
	COMPONENT_TYPE Type = COMPONENT_TYPE::END;
	while (true)
	{
		// ����Ǿ��ִ� ������ � ������Ʈ���� Ȯ��
		fread(&Type, sizeof(COMPONENT_TYPE), 1, _File);

		// ���� Ÿ�� ������ END �� ���, ����� ������Ʈ ������ ���̴�.
		if (COMPONENT_TYPE::END == Type)
			break;

		// ����� Ÿ�Կ� �´� ������Ʈ�� ���� ��Ű��, �����Ҷ��� ������ ������ �����͸� �д´�.
		DComponent* pComponent = GetComponent(Type);

		// ������ ������Ʈ�� ������Ʈ�� �־��ش�.
		pObject->AddComponent(pComponent);

		// �������� ������ �о�ͼ� �����Ѵ�.
		pComponent->LoadFromFile(_File);
	}

	// Script ���� �ε�	
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		// Script �� �̸� �б�
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// ���� Script �̸����� �̸��� �ش��ϴ� Script ����
		DScript* pScript = DScriptMgr::GetScript(ScriptName);
		pScript->LoadFromFile(_File);

		pObject->AddComponent(pScript);
	}

	// Child ���� �ε�
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
