#include "pch.h"
#include "DPaperFlipbook.h"

// Device Headers
#include "DDevice.h"
#include "DConstantBuffer.h"

// Manager Headers
#include "DTimeMgr.h"
#include "DAssetMgr.h"

// Asset Headers
#include "DFlipbook.h"

DPaperFlipbook::DPaperFlipbook()
	: DComponent(COMPONENT_TYPE::PAPERFLIPBOOK)
	, m_CurFlipbook(nullptr)
	, m_CurFrmIdx(0)
{
}

DPaperFlipbook::DPaperFlipbook(DPaperFlipbook& _Origin)
	: DComponent(_Origin)
	, m_vecFlipbook(_Origin.m_vecFlipbook)
	, m_CurFlipbook(_Origin.m_CurFlipbook)
	, m_CurFrmIdx(0)
	, m_FPS(_Origin.m_FPS)
	, m_AccTime(0.f)
	, m_Repeat(_Origin.m_Repeat)
	, m_Finish(false)
{
	if (nullptr != m_CurFlipbook)
	{
		int FlipbookIdx = 0;
		for (; FlipbookIdx < (int)m_vecFlipbook.size(); ++FlipbookIdx)
		{
			if (m_CurFlipbook == m_vecFlipbook[FlipbookIdx])
				break;
		}

		Play(FlipbookIdx, m_FPS, m_Repeat);
	}
}

DPaperFlipbook::~DPaperFlipbook()
{
}

void DPaperFlipbook::FinalTick()
{
	if (m_Finish)
	{
		if (false == m_Repeat)
			return;

		Reset();
	}

	if (nullptr != m_CurFlipbook)
	{
		float MaxTime = 1.f / m_FPS;

		m_AccTime += DT;

		if (MaxTime < m_AccTime)
		{
			m_AccTime -= MaxTime;
			++m_CurFrmIdx;

			if (m_CurFlipbook->GetMaxFrameCount() <= m_CurFrmIdx)
			{
				--m_CurFrmIdx;
				m_Finish = true;
			}
		}

		m_CurFrmSprite = m_CurFlipbook->GetSprite(m_CurFrmIdx);
	}
}

void DPaperFlipbook::AddFlipbook(int _Idx, Ptr<DFlipbook> _Flipbook)
{
	if (m_vecFlipbook.size() <= _Idx)
	{
		m_vecFlipbook.resize(_Idx + 1);
	}

	m_vecFlipbook[_Idx] = _Flipbook;
}

Ptr<DFlipbook> DPaperFlipbook::FindFlipbook(const wstring& _Key)
{
	for (size_t i = 0; i < m_vecFlipbook.size(); ++i)
	{
		if (m_vecFlipbook[i]->GetKey() == _Key)
			return m_vecFlipbook[i];
	}

	return nullptr;
}

void DPaperFlipbook::Play(int _FliBookIdx, float _FPS, bool _Repeat)
{
	m_CurFlipbook = m_vecFlipbook[_FliBookIdx];

	if (nullptr == m_CurFlipbook)
	{
		return;
	}

	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_FPS = _FPS;
	m_Repeat = _Repeat;
}

void DPaperFlipbook::Reset()
{
	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_Finish = false;
}

void DPaperFlipbook::Binding()
{
	if (nullptr != m_CurFrmSprite)
	{
		// 현재 표시해야하는 Sprite 의 정보를 Sprite 전용 상수버퍼를 통해서 GPU 에 전달
		tSpriteInfo tInfo = {};

		tInfo.LeftTopUV = m_CurFrmSprite->GetLeftTopUV();
		tInfo.SliceUV = m_CurFrmSprite->GetSliceUV();
		tInfo.BackGroundUV = m_CurFrmSprite->GetBackgroundUV();
		tInfo.OffsetUV = m_CurFrmSprite->GetOffsetUV();
		tInfo.UseFlipbook = 1;

		static DConstantBuffer* CB = DDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);

		CB->SetData(&tInfo);
		CB->Binding();

		// Flipbook Sprite 아틀라스 텍스쳐 전용 레지스터번호 t10 에 바인딩
		Ptr<DTexture> pAtlas = m_CurFrmSprite->GetAtlasTexture();
		pAtlas->Binding(10);
	}
	else
	{
		Clear();
	}
}

void DPaperFlipbook::Clear()
{
	tSpriteInfo tInfo = {};
	static DConstantBuffer* CB = DDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);
	CB->SetData(&tInfo);
	CB->Binding();
}

void DPaperFlipbook::SaveToFile(FILE* _File)
{
	// Flipbook 에셋 목록 저장
	size_t FlipbookCount = m_vecFlipbook.size();
	fwrite(&FlipbookCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < m_vecFlipbook.size(); ++i)
	{
		SaveAssetRef(m_vecFlipbook[i], _File);
	}

	// 현재 재생중인 Flipbook 정보 저장
	SaveAssetRef(m_CurFlipbook, _File);

	// 현재 재생중인 Flipbook 내에서 지정된 Sprite
	SaveAssetRef(m_CurFrmSprite, _File);

	// 현재 재생중인 Flipbook 내에서 지정된 Sprite 가 몇번째 인덱스인지
	fwrite(&m_CurFrmIdx, sizeof(int), 1, _File);
	fwrite(&m_FPS, sizeof(float), 1, _File);
	fwrite(&m_AccTime, sizeof(float), 1, _File);
	fwrite(&m_Repeat, sizeof(bool), 1, _File);
}

void DPaperFlipbook::LoadFromFile(FILE* _File)
{
	// Flipbook 에셋 목록 불러오기
	size_t FlipbookCount = 0;
	fread(&FlipbookCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < FlipbookCount; ++i)
	{
		Ptr<DFlipbook> pFlipbook;
		LoadAssetRef(pFlipbook, _File);
		m_vecFlipbook.push_back(pFlipbook);
	}

	// 현재 재생중인 Flipbook 정보 로드
	LoadAssetRef(m_CurFlipbook, _File);

	// 현재 재생중인 Flipbook 내에서 지정된 Sprite
	LoadAssetRef(m_CurFrmSprite, _File);

	// 현재 재생중인 Flipbook 내에서 지정된 Sprite 가 몇번째 인덱스인지
	fread(&m_CurFrmIdx, sizeof(int), 1, _File);
	fread(&m_FPS, sizeof(float), 1, _File);
	fread(&m_AccTime, sizeof(float), 1, _File);
	fread(&m_Repeat, sizeof(bool), 1, _File);
}
