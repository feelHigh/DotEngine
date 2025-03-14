#include "pch.h"
#include "DAssetMgr.h"

// Device Headers
#include "DDevice.h"

void DAssetMgr::Init()
{
	CreateEngineMesh();

	CreateEngineTexture();

	CreateEngineSprite();

	CreateEngineGraphicShader();

	CreateEngineComputeShader();

	CreateEngineMaterial();
}


void DAssetMgr::CreateEngineMesh()
{
	Ptr<DMesh> pMesh = nullptr;
	tVtx v;

	// PointMesh
	pMesh = new DMesh;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT i = 0;
	pMesh->Create(&v, 1, &i, 1);
	AddAsset(L"PointMesh", pMesh);

	// RectMesh
	// 0 -- 1
	// | \  |
	// 3 -- 2
	tVtx arrVtx[4] = {};

	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);


	// Create Index Buffer
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;
	arrIdx[3] = 0; 	arrIdx[4] = 2;	arrIdx[5] = 3;

	pMesh = new DMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh", pMesh);


	// RectMesh_Debug
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2; arrIdx[3] = 3; arrIdx[4] = 0;

	pMesh = new DMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh_Debug", pMesh);


	// CircleMesh 
	vector<tVtx> vecVtx;
	vector<UINT> vecIdx;


	int Slice = 40;
	float fTheta = XM_2PI / Slice;
	float Radius = 0.5f;

	// Central point
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	// Border
	float Angle = 0.f;
	for (int i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -(v.vPos.y - 0.5f));
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Angle += fTheta;
	}

	// Index
	for (int i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new DMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh", pMesh);

	// CircleMesh_Debug
	vecIdx.clear();
	for (size_t i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back((UINT)i);
	}

	pMesh = new DMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh_Debug", pMesh);
}

void DAssetMgr::CreateEngineTexture()
{
	// Create textures for Post Process usage
	Vec2 Resolution = DDevice::GetInst()->GetResolution();
	Ptr<DTexture> pPostProcessTex = CreateTexture(L"PostProcessTex"
												, (UINT)Resolution.x, (UINT)Resolution.y
												, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	Ptr<DTexture> pEffectTarget = CreateTexture(L"EffectTargetTex"
												, (UINT)(Resolution.x), (UINT)(Resolution.y)
												, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

	Ptr<DTexture> pEffectDepth = CreateTexture(L"EffectDepthStencilTex"
												, (UINT)(Resolution.x), (UINT)(Resolution.y)
												, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	Ptr<DTexture> pEffectBlurTarget = CreateTexture(L"EffectBlurTargetTex"
													, (UINT)(Resolution.x), (UINT)(Resolution.y)
													, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
}

void DAssetMgr::CreateEngineSprite()
{
}

void DAssetMgr::CreateEngineGraphicShader()
{
	Ptr<DGraphicShader> pShader = nullptr;
	//-----------------------------------------------------------------------------------------------------
	// Std2DShader
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.hlsl", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.hlsl", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	pShader->AddTexParam(TEX_0, "OutputTexture");

	AddAsset(L"Std2DShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// Std2DAlphaBlend
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.hlsl", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.hlsl", "PS_Std2D_Alphablend");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"Std2DAlphaBlendShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// EffectShader
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.hlsl", "VS_Effect");
	pShader->CreatePixelShader(L"Shader\\std2d.hlsl", "PS_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	AddAsset(L"EffectShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// DebugShapeShader
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\Debug.hlsl", "VS_DebugShape");
	pShader->CreatePixelShader(L"Shader\\Debug.hlsl", "PS_DebugShape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);

	AddAsset(L"DebugShapeShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// TileMapShader
	pShader = new DGraphicShader;

	pShader->CreateVertexShader(L"Shader\\TileMap.hlsl", "VS_TileMap");
	pShader->CreatePixelShader(L"Shader\\TileMap.hlsl", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(L"TileMapShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// ParticleShader
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\Particle.hlsl", "VS_Particle");
	pShader->CreateGeometryShader(L"Shader\\Particle.hlsl", "GS_Particle");
	pShader->CreatePixelShader(L"Shader\\Particle.hlsl", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	AddAsset(L"ParticleRenderShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// GrayFilterShader
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\PostProcess.hlsl", "VS_GrayFilter");
	pShader->CreatePixelShader(L"Shader\\PostProcess.hlsl", "PS_GrayFilter");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"GrayFilterShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// DistortionShader
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\PostProcess.hlsl", "VS_Distortion");
	pShader->CreatePixelShader(L"Shader\\PostProcess.hlsl", "PS_Distortion");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"DistortionShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// BlurShader
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\PostProcess.hlsl", "VS_Blur");
	pShader->CreatePixelShader(L"Shader\\PostProcess.hlsl", "PS_Blur");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"BlurShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// EffectMerge
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\PostProcess.hlsl", "VS_EffectMerge");
	pShader->CreatePixelShader(L"Shader\\PostProcess.hlsl", "PS_EffectMerge");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"EffectMergeShader", pShader);
	//-----------------------------------------------------------------------------------------------------
	// TileMapEditorShader
	pShader = new DGraphicShader;
	pShader->CreateVertexShader(L"Shader\\TE_EditorShader.hlsl", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\TE_EditorShader.hlsl", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	pShader->AddTexParam(TEX_0, "OutputTexture");

	AddAsset(L"TE_EditorShader", pShader);
}

#include "DParticleTickCS.h"

void DAssetMgr::CreateEngineComputeShader()
{
	// ParticleTick
	Ptr<DComputeShader> pCS = nullptr;

	pCS = new DParticleTickCS;
	AddAsset<DComputeShader>(L"ParticleTickCS", pCS);
}

void DAssetMgr::CreateEngineMaterial()
{
	Ptr<DMaterial>	pMtrl = nullptr;
	//-----------------------------------------------------------------------------------------------------
	// Std2DMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"Std2DShader"));
	AddAsset(L"Std2DMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// Std2DMtrl-2
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"Std2DShader"));
	AddAsset(L"Std2DMtrl2", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// Std2DMtrl-3
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"Std2DShader"));
	AddAsset(L"Std2DMtrl3", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// Std2DAlphaBlendMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"Std2DAlphaBlendShader"));
	AddAsset(L"Std2DAlphaBlendMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// EffectMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"EffectShader"));
	AddAsset(L"EffectMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// DebugShapeMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"DebugShapeShader"));
	AddAsset(L"DebugShapeMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// TileMapMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"TileMapShader"));
	AddAsset(L"TileMapMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// ParticleRenderMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"ParticleRenderShader"));
	AddAsset(L"ParticleRenderMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// GrayFilterMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"GrayFilterShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<DTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<DTexture>(L"Texture\\Noise\\noise_01.png"));
	pMtrl->SetTexParam(TEX_2, FindAsset<DTexture>(L"Texture\\Noise\\noise_02.png"));
	pMtrl->SetTexParam(TEX_3, FindAsset<DTexture>(L"Texture\\Noise\\noise_03.jpg"));
	AddAsset(L"GrayFilterMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// DistortionMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"DistortionShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<DTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<DTexture>(L"Texture\\Noise\\noise_01.png"));
	pMtrl->SetTexParam(TEX_2, FindAsset<DTexture>(L"Texture\\Noise\\noise_02.png"));
	pMtrl->SetTexParam(TEX_3, FindAsset<DTexture>(L"Texture\\Noise\\noise_03.jpg"));
	AddAsset(L"DistortionMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// BlurMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"BlurShader"));
	AddAsset(L"BlurMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// EffectMergeMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"EffectMergeShader"));
	AddAsset(L"EffectMergeMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
	// TE_EditorMtrl
	pMtrl = new DMaterial(true);
	pMtrl->SetShader(FindAsset<DGraphicShader>(L"TE_EditorShader"));
	AddAsset(L"TE_EditorMtrl", pMtrl);
	//-----------------------------------------------------------------------------------------------------
}
