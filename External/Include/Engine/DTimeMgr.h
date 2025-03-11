#pragma once
#include "Singleton.h"

class DTimeMgr
	: public DSingleton<DTimeMgr>
{
	SINGLE(DTimeMgr);
public:
	void Init();
	void Tick();
	void Render();

public:
	float GetDeltaTime() { return m_DeltaTime; }
	float GetTime() { return m_Time; }

	float GetEngineDeltaTime() { return m_E_DeltaTime; }
	float GetEngineTime() { return m_E_Time; }

	UINT GetFPS() { return m_FPS; }

private:
	// LARGE_INTEGER - 8 byte integer substitute
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_FPS;

	float			m_DeltaTime;	// Frame interval time (time taken to execute 1 frame)
	float			m_Time;			// Time since the program has been turned on

	float			m_E_DeltaTime;
	float			m_E_Time;

	wchar_t			m_TimeInfo[255];

};
