#ifndef ENGINE_DTASKMGR_H
#define ENGINE_DTASKMGR_H

#include "Singleton.h"

class DTaskMgr
	: public DSingleton<DTaskMgr>
{
	SINGLE(DTaskMgr);
public:
	void Tick();

public:
	void AddTask(const tTask& _Task);

private:
	void ClearGC();
	void ExecuteTask();

private:
	vector<tTask>			m_vecTask;
	vector<DGameObject*>	m_GC; // Garbage Collector;

};

#endif // !ENGINE_DTASKMGR_H