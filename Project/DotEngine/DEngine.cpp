#include "pch.h"
#include "DEngine.h"

// Device Headers
#include "DDevice.h"

// Manager Headers
#include "DPathMgr.h"
#include "DKeyMgr.h"
#include "DTimeMgr.h"
#include "DAssetMgr.h"
#include "DLevelMgr.h"
#include "DRenderMgr.h"
#include "DCollisionMgr.h"
#include "DTaskMgr.h"
#include "DFontMgr.h"

// Asset Headers
#include "DPrefab.h"

DEngine::DEngine()
	: m_hWnd(nullptr)
	, m_ptResolution{}
{
}

DEngine::~DEngine()
{
	if (nullptr != m_FMODSystem)
	{
		m_FMODSystem->release();
		m_FMODSystem = nullptr;
	}
}

int DEngine::Init(HWND _wnd, POINT _ptResolution, OBJECT_SAVE _SaveFunc, OBJECT_LOAD _Func)
{
	m_hWnd = _wnd;
	m_ptResolution = _ptResolution;
	ChangeWindowScale(_ptResolution.x, _ptResolution.y);

	if (FAILED(DDevice::GetInst()->Init(m_hWnd, m_ptResolution.x, m_ptResolution.y)))
	{
		MessageBox(nullptr, L"Device initialization failed.", L"Device initialization failed.(DEngine)", MB_OK);
		return E_FAIL;
	}

	// FMOD 초기화		
	FMOD::System_Create(&m_FMODSystem);
	assert(m_FMODSystem);

	// 32개 채널 생성
	m_FMODSystem->init(32, FMOD_DEFAULT, nullptr);


	// Manager 초기화
	DPathMgr::GetInst()->Init();
	DKeyMgr::GetInst()->Init();
	DTimeMgr::GetInst()->Init();
	DAssetMgr::GetInst()->Init();
	DRenderMgr::GetInst()->Init();
	DLevelMgr::GetInst()->Init();
	DFontMgr::GetInst()->Init();


	// Prefab Function 등록
	DPrefab::g_ObjectSaveFunc = _SaveFunc;
	DPrefab::g_ObjectLoadFunc = _Func;

	return S_OK;
}

void DEngine::Progress()
{
	// FMOD Tick
	m_FMODSystem->update();

	// Manager
	DKeyMgr::GetInst()->Tick();
	DTimeMgr::GetInst()->Tick();
	DAssetMgr::GetInst()->Tick();
	DLevelMgr::GetInst()->Progress();

	// Collision
	DCollisionMgr::GetInst()->Tick();

	// Render	
	DRenderMgr::GetInst()->Tick();

	// TaskMgr
	DTaskMgr::GetInst()->Tick();
}

void DEngine::ChangeWindowScale(UINT _Width, UINT _Height)
{
	bool bMenu = false;
	if (GetMenu(m_hWnd))
		bMenu = true;

	RECT rt = { 0, 0, (int)_Width, (int)_Height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, bMenu);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}
