#pragma once
#include "Singleton.h"

class DLevel;
class DGameObject;

class DLevelMgr
	: public DSingleton<DLevelMgr>
{
	SINGLE(DLevelMgr);
public:
	friend class DTaskMgr;

public:
	void Init();
	void Progress();

public:
	DLevel* GetCurrentLevel() { return m_CurLevel; }
	DGameObject* FindObjectByName(const wstring& _Name);

	void LevelChanged();
	bool IsLevelChanged() { return m_LevelChanged; }

private:
	bool ChangeLevel(DLevel* _NextLevel);

private:
	DLevel*		m_CurLevel;
	bool		m_LevelChanged;

};
