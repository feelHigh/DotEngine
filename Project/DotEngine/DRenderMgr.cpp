#include "pch.h"
#include "DRenderMgr.h"

// Device Headers
#include "DDevice.h"
#include "DConstantBuffer.h"
#include "DStructuredBuffer.h"

// Manager Headers
#include "DTimeMgr.h"
#include "DAssetMgr.h"
#include "DLevelMgr.h"
#include "DKeyMgr.h"

// Level Headers
#include "DLevel.h"

// GameObejct Heders
#include "DGameObject.h"

// Component Headers
#include "DCamera.h"
#include "DTransform.h"
#include "DLight2D.h"
#include "DMeshRender.h"

DRenderMgr::DRenderMgr()
	: m_EditorCamera(nullptr)
	, m_Light2DBuffer(nullptr)
{
	m_Light2DBuffer = new DStructuredBuffer;
}

DRenderMgr::~DRenderMgr()
{
	if (nullptr != m_DebugObject)
		delete m_DebugObject;

	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;
}

void DRenderMgr::Init()
{
	// Refer to the texture for post-processing created when AssetMgr is initialized.
	m_PostProcessTex = DAssetMgr::GetInst()->FindAsset<DTexture>(L"PostProcessTex");

	// Game Object for Debug Rendering
	m_DebugObject = new DGameObject;
	m_DebugObject->AddComponent(new DTransform);
	m_DebugObject->AddComponent(new DMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(DAssetMgr::GetInst()->FindAsset<DMaterial>(L"DebugShapeMtrl"));
}

void DRenderMgr::Tick()
{
	DLevel* pCurLevel = DLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	RenderStart();

	// If Level is in Player state, render to camera view within Level
	if (PLAY == pCurLevel->GetState())
	{
		for (size_t i = 0; i < m_vecCam.size(); ++i)
		{
			if (nullptr == m_vecCam[i])
				continue;

			m_vecCam[i]->Render();
		}
	}

	// If the Level is Stop or Pause, render to camera view for Editor
	else
	{
		if (nullptr != m_EditorCamera)
		{
			m_EditorCamera->Render();
		}
	}

	// Debug Render
	RenderDebugShape();

	// Time information rendering
	DTimeMgr::GetInst()->Render();

	// Clear
	Clear();
}

void DRenderMgr::RegisterCamera(DCamera* _Cam, int _CamPriority)
{
	// A space for vectors should be provided according to the camera priority.
	if (m_vecCam.size() <= _CamPriority + 1)
		m_vecCam.resize(_CamPriority + 1);

	// Put it in a position that matches the priority of the camera
	m_vecCam[_CamPriority] = _Cam;
}

void DRenderMgr::PostProcessCopy()
{
	// RenderTarget -> PostProcessTex
	Ptr<DTexture> pRTTex = DAssetMgr::GetInst()->FindAsset<DTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}


void DRenderMgr::RenderStart()
{
	// Set Render Target
	Ptr<DTexture> pRTTex = DAssetMgr::GetInst()->FindAsset<DTexture>(L"RenderTargetTex");
	Ptr<DTexture> pDSTex = DAssetMgr::GetInst()->FindAsset<DTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// TargetClear
	float color[4] = { 0.f, 0.f, 0.f, 1.f };
	CONTEXT->ClearRenderTargetView(pRTTex->GetRTV().Get(), color);
	CONTEXT->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	g_GlobalData.g_Resolution = Vec2((float)pRTTex->GetWidth(), (float)pRTTex->GetHeight());
	g_GlobalData.g_Light2DCount = (int)m_vecLight2D.size();

	// Update and bind Light2D information
	vector<tLightInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	m_Light2DBuffer->SetData(vecLight2DInfo.data());
	m_Light2DBuffer->Binding(11);


	// Global Data Binding
	static DConstantBuffer* pGlobalCB = DDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_GlobalData);
	pGlobalCB->Binding();
}

void DRenderMgr::Clear()
{
	m_vecLight2D.clear();
}

void DRenderMgr::RenderDebugShape()
{
	list<tDebugShapeInfo>::iterator iter = m_DebugShapeList.begin();

	for (; iter != m_DebugShapeList.end(); )
	{
		// Gets the mesh that matches the debug request shape
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->MeshRender()->SetMesh(DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->MeshRender()->SetMesh(DAssetMgr::GetInst()->FindAsset<DMesh>(L"CircleMesh_Debug"));
			break;
		}

		// Position setting
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// Material setting
		m_DebugObject->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

		// According to whether or not the depth is determined, the depth determination method of the shader is determined
		if ((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// Rendering
		m_DebugObject->MeshRender()->Render();


		// Delete end-of-life debug information
		(*iter).Age += EngineDT;
		if ((*iter).LifeTime < (*iter).Age)
		{
			iter = m_DebugShapeList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
