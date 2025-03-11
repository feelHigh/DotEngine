#include "pch.h"
#include "DTestLevel.h"

// Base Headers
#include <Engine\AssetList.h>
#include <Engine\ComponentList.h>

// Device Headers
#include <Engine\DStructuredBuffer.h>

// Manager Headers
#include <Engine\DLevelMgr.h>
#include <Engine\DAssetMgr.h>
#include <Engine\DCollisionMgr.h>

// Level Headers
#include <Engine\DLevel.h>
#include <Engine\DLayer.h>

// GameObject Headers
#include <Engine\DGameObject.h>

// Module Headers
#include <Engine\DLevelSaveLoad.h>

// Shader Code
#include <Engine\DSetColorCS.h>

// Scripts
#include <Scripts\PlayerScript.h>
#include <Scripts\MissileScript.h>
#include <Scripts\CameraMoveScript.h>


void DTestLevel::CreateTestLevel()
{
	// Material
	Ptr<DMaterial> pMtrl = DAssetMgr::GetInst()->FindAsset<DMaterial>(L"Std2DMtrl");
	Ptr<DMaterial> pAlphaBlendMtrl = DAssetMgr::GetInst()->FindAsset<DMaterial>(L"Std2DAlphaBlendMtrl");
	Ptr<DMaterial> pDebugShapeMtrl = DAssetMgr::GetInst()->FindAsset<DMaterial>(L"DebugShapeMtrl");

	Ptr<DTexture> pTexture = DAssetMgr::GetInst()->FindAsset<DTexture>(L"Texture\\Character.png");
	pAlphaBlendMtrl->SetTexParam(TEX_0, pTexture);

	CreatePrefab();

	// Sound 재생
	Ptr<DSound> pSound = DAssetMgr::GetInst()->FindAsset<DSound>(L"Sound\\1_MainTheme.wav");
	//pSound->Play(0, 0.4f, false);

	// Level 생성
	DLevel* pLevel = new DLevel;

	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Background");
	pLevel->GetLayer(2)->SetName(L"Foreground");
	pLevel->GetLayer(3)->SetName(L"Player");
	pLevel->GetLayer(4)->SetName(L"Monster");
	pLevel->GetLayer(5)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(6)->SetName(L"MonsterProjectile");

	// 카메라 오브젝트
	DGameObject* CamObj = new DGameObject;
	CamObj->SetName(L"MainCamera");
	CamObj->AddComponent(new DTransform);
	CamObj->AddComponent(new DCamera);
	CamObj->AddComponent(new CameraMoveScript);

	// 우선순위를 0 : MainCamera 로 설정
	CamObj->Camera()->SetPriority(0);

	// 카메라 레이어 설정 (31번 레이어 제외 모든 레이어를 촬영)
	CamObj->Camera()->SetLayerAll();
	CamObj->Camera()->SetLayer(31, false);
	CamObj->Camera()->SetFar(100000.f);
	CamObj->Camera()->SetProjType(ORTHOGRAPHIC);

	pLevel->AddObject(0, CamObj);

	DGameObject* pObject = nullptr;


	// 광원 오브젝트 추가
	pObject = new DGameObject;
	pObject->SetName(L"World Light");
	pObject->AddComponent(new DTransform);
	pObject->AddComponent(new DLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pObject->Light2D()->SetRadius(1000.f);
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	pLevel->AddObject(0, pObject);

	// 배경 오브젝트
	DGameObject* pBG = new DGameObject;
	pBG->SetName(L"BG");
	pBG->AddComponent(new DTransform);
	pBG->AddComponent(new DMeshRender);

	pBG->Transform()->SetRelativePos(0.f, 0.f, 500.f);
	pBG->Transform()->SetRelativeScale(1920.f, 1920.f, 1.f);

	pBG->MeshRender()->SetMesh(DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh"));
	pBG->MeshRender()->SetMaterial(DAssetMgr::GetInst()->FindAsset<DMaterial>(L"Std2DMtrl"));

	//pBG->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(2.f, 10.f, 4.f, 1.f));
	pBG->MeshRender()->GetMaterial()->SetTexParam(TEX_0, DAssetMgr::GetInst()->FindAsset<DTexture>(L"Texture\\Background\\Title1920-Alpha.png"));

	pLevel->AddObject(1, pBG);

	DGameObject* pFG = new DGameObject;
	pFG->SetName(L"FG");
	pFG->AddComponent(new DTransform);
	pFG->AddComponent(new DMeshRender);

	pFG->Transform()->SetRelativePos(0.f, 158.f, 400.f);
	pFG->Transform()->SetRelativeScale(384.f, 192.f, 1.f);

	pFG->MeshRender()->SetMesh(DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh"));
	//pFG->MeshRender()->SetMaterial(DAssetMgr::GetInst()->FindAsset<DMaterial>(L"Std2DMtrl"));
	pFG->MeshRender()->SetMaterial(DAssetMgr::GetInst()->FindAsset<DMaterial>(L"Std2DMtrl2"));

	//pFG->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(2.f, 10.f, 4.f, 1.f));
	pFG->MeshRender()->GetMaterial()->SetTexParam(TEX_0, DAssetMgr::GetInst()->FindAsset<DTexture>(L"Texture\\Background\\logo_white-Alpha.png"));

	pLevel->AddObject(2, pFG);

	// 플레이어 오브젝트
	/*DGameObject* pPlayer = new DGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new DTransform);
	pPlayer->AddComponent(new DMeshRender);
	pPlayer->AddComponent(new DCollider2D);
	pPlayer->AddComponent(new DPaperFlipbook);
	pPlayer->AddComponent(new PlayerScript);
	pPlayer->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pPlayer->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

	pPlayer->Collider2D()->SetIndependentScale(false);
	pPlayer->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pPlayer->Collider2D()->SetScale(Vec3(1.f, 1.f, 1.f));

	pPlayer->MeshRender()->SetMesh(DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh"));
	pPlayer->MeshRender()->SetMaterial(DAssetMgr::GetInst()->FindAsset<DMaterial>(L"Std2DMtrl3"));

	Ptr<DFlipbook> pFlipbook = DAssetMgr::GetInst()->FindAsset<DFlipbook>(L"Flipbook\\Idle_Left.flip");
	pPlayer->PaperFlipbook()->AddFlipbook(1, pFlipbook);
	pPlayer->PaperFlipbook()->Play(1, 6, true);

	pLevel->AddObject(3, pPlayer);*/


	// Effect Object
	/*DGameObject* pMonster = new DGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new DTransform);
	pMonster->AddComponent(new DMeshRender);

	pMonster->Transform()->SetRelativePos(50.f, 0.f, 100.f);
	pMonster->Transform()->SetRelativeScale(300.f, 150.f, 1.f);

	pMonster->MeshRender()->SetMesh(DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetMaterial(DAssetMgr::GetInst()->FindAsset<DMaterial>(L"EffectMtrl"));

	pMonster->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(2.f, 10.f, 4.f, 1.f));
	pMonster->MeshRender()->GetMaterial()->SetTexParam(TEX_0, DAssetMgr::GetInst()->FindAsset<DTexture>(L"Texture\\fire_11.png"));

	pLevel->AddObject(4, pMonster);*/

	// TileMap Object
	DGameObject* pTileMapObj = new DGameObject;
	pTileMapObj->SetName(L"TileMap");

	pTileMapObj->AddComponent(new DTransform);
	pTileMapObj->AddComponent(new DTileMap);

	pTileMapObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));

	pTileMapObj->TileMap()->SetRowCol(5, 5);
	pTileMapObj->TileMap()->SetTileSize(Vec2(64.f, 64.f));

	Ptr<DTexture> pTileAtlas = DAssetMgr::GetInst()->FindAsset<DTexture>(L"Texture\\TILE.bmp");
	pTileMapObj->TileMap()->SetAtlasTexture(pTileAtlas);
	pTileMapObj->TileMap()->SetAtlasTileSize(Vec2(64.f, 64.f));

	//pTileMapObj->TileMap()->SetTile(0, 0, 1);	// SetTile Test 

	pLevel->AddObject(4, pTileMapObj);

	// Particle Object
	/*DGameObject* pParticleObj = new DGameObject;
	pParticleObj->SetName(L"Particle");

	pParticleObj->AddComponent(new DTransform);
	pParticleObj->AddComponent(new DParticleSystem);

	pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));

	pLevel->AddObject(0, pParticleObj);*/


	// PostProcess Object
	//DGameObject* pGrayFilterObj = new DGameObject;
	//pGrayFilterObj->SetName(L"GrayFilter");
	//pGrayFilterObj->AddComponent(new DTransform);
	//pGrayFilterObj->AddComponent(new DMeshRender);

	//pGrayFilterObj->Transform()->SetRelativeScale(150.f, 150.f, 1.f);

	//pGrayFilterObj->MeshRender()->SetMesh(DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh"));
	//pGrayFilterObj->MeshRender()->SetMaterial(DAssetMgr::GetInst()->FindAsset<DMaterial>(L"DistortionMtrl"));

	//pLevel->AddObject(0, pGrayFilterObj);

	ChangeLevel(pLevel, LEVEL_STATE::STOP);

	// 충돌 지정
	DCollisionMgr::GetInst()->CollisionCheck(3, 4); // Player vs Monster
	DCollisionMgr::GetInst()->CollisionCheck(5, 4); // Player Projectile vs Monster	
}

void DTestLevel::CreatePrefab()
{
	/*DGameObject* pProto = new DGameObject;
	pProto->SetName("Missile");
	pProto->AddComponent(new DTransform);
	pProto->AddComponent(new DMeshRender);
	pProto->AddComponent(new DMissileScript);

	pProto->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pProto->MeshRender()->SetMesh(DAssetMgr::GetInst()->FindAsset<DMesh>(L"RectMesh"));
	pProto->MeshRender()->SetMaterial(DAssetMgr::GetInst()->FindAsset<DMaterial>(L"Std2DMtrl"));

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetProtoObject(pProto);

	DAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePref", pPrefab);

	wstring FilePath = CPathMgr::GetInst()->GetContentPath();
	FilePath += L"Prefab\\Missile.pref";
	pPrefab->Save(FilePath);*/
}
