#include "pch.h"
#include "DTimeMgr.h"

// Engine Headers
#include "DEngine.h"

// Manager Headers
#include "DKeyMgr.h"
#include "DFontMgr.h"
#include "DLevelMgr.h"

// Level Headers
#include "DLevel.h"

DTimeMgr::DTimeMgr()
	: m_llCurCount{}
	, m_llPrevCount{}
	, m_llFrequency{}
	, m_FPS(0)
	, m_DeltaTime(0.f)
	, m_Time(0.f)
	, m_E_DeltaTime(0.f)
	, m_E_Time(0.f)
	, m_TimeInfo{}
{
}

DTimeMgr::~DTimeMgr()
{
}

void DTimeMgr::Init()
{
	// The GetTickCount function, which counts 1000 per second, is less accurate to measure fine time.

	// Get count criteria that can be counted per second.
	QueryPerformanceFrequency(&m_llFrequency);

	QueryPerformanceCounter(&m_llCurCount);
	m_llPrevCount = m_llCurCount;
}

void DTimeMgr::Tick()
{
	// Calculate the current count
	QueryPerformanceCounter(&m_llCurCount);

	// Calculate the time value between one frame using the difference between the previous and current counts
	m_E_DeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// DT calibration
	if (1.f / 60.f < m_E_DeltaTime)
		m_E_DeltaTime = 1.f / 60.f;

	// Record the value of time since the program was executed through cumulative time
	m_E_Time += m_E_DeltaTime;

	// Copy the current count value to the previous count
	m_llPrevCount = m_llCurCount;

	// Calculate number of runs per second (FPS)
	++m_FPS;

	// TextOut output once per second
	static float AccTime = 0.f;
	AccTime += m_E_DeltaTime;

	if (1.f < AccTime)
	{
		swprintf_s(m_TimeInfo, L"DeltaTime : %f, FPS : %d ", m_E_DeltaTime, m_FPS);
		//TextOut(DEngine::GetInst()->GetMainDC(), 10, 10, szBuff, wcslen(szBuff));		
		AccTime = 0.f;
		m_FPS = 0;
	}

	// DT for Level
	DLevel* pCurLevel = DLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel || pCurLevel->GetState() != LEVEL_STATE::PLAY)
	{
		m_DeltaTime = 0.f;
	}
	else
	{
		m_DeltaTime = m_E_DeltaTime;
	}

	m_Time += m_DeltaTime;


	// GlobalData
	g_GlobalData.g_DT = m_DeltaTime;
	g_GlobalData.g_EngineDT = m_E_DeltaTime;
	g_GlobalData.g_Time = m_Time;
	g_GlobalData.g_EngineTime = m_E_Time;

	if (KEY_PRESSED(KEY::RBTN))
	{
		g_GlobalData.g_EngineDT = 0.f;
	}
}

void DTimeMgr::Render()
{
	DFontMgr::GetInst()->DrawFont(m_TimeInfo, 10, 20, 16, FONT_RGBA(255, 20, 20, 255));
}
