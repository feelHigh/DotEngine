#pragma once

class DLevelSaveLoad
{
public:
	static void SaveLevel(const wstring& _FilePath, DLevel* _Level);
	static void SaveGameObject(FILE* _File, DGameObject* _Object);

	static class DLevel* LoadLevel(const wstring& _FilePath);
	static class DGameObject* LoadGameObject(FILE* _File);
	static class DComponent* GetComponent(COMPONENT_TYPE _Type);
};
