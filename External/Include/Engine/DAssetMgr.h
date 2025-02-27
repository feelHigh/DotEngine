#ifndef ENGINE_DASSETMGR_H
#define ENGINE_DASSETMGR_H

#include "Singleton.h"

#include "DPathMgr.h"
#include "DTaskMgr.h"

class DAsset;

class DAssetMgr
	: public DSingleton<DAssetMgr>
{
	SINGLE(DAssetMgr);
public:
	friend class DTaskMgr;

public:
	void Init();
	void Tick();

private:
	void CreateEngineMesh();
	void CreateEngineMaterial();
	void CreateEngineTexture();
	void CreateEngineSprite();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();

public:
	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath);

	Ptr<DAsset> FindAsset(ASSET_TYPE _Type, const wstring& _Key);
	//void AddAsset(const wstring& _Key, DAsset* _Asset);

	template<typename T>
	Ptr<T> FindAsset(const wstring& _Key);

	template<typename T>
	void AddAsset(const wstring& _Key, Ptr<T> _Asset);

	// _Flags : D3D11_BIND_FLAG
	Ptr<DTexture> CreateTexture(wstring _strKey, UINT _Width, UINT _Height
								, DXGI_FORMAT _Format, UINT _Flags
								, D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);

	Ptr<DTexture> CreateTexture(wstring _strKey, ComPtr<ID3D11Texture2D> _Tex2D);

public:
	void GetAssetNames(ASSET_TYPE _Type, vector<string>& _vecOut);
	const map<wstring, Ptr<DAsset>>& GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(UINT)_Type]; }
	bool IsChanged() { return m_Changed; }

private:
	void DeleteAsset(ASSET_TYPE _Type, const wstring& _Key);

private:
	map<wstring, Ptr<DAsset>>	m_mapAsset[(UINT)ASSET_TYPE::END];
	bool						m_Changed;

};


template<typename T>
Ptr<T> DAssetMgr::Load(const wstring& _Key, const wstring& _RelativePath)
{
	// 동일 키값 에셋이 있는지 확인
	Ptr<T> Asset = FindAsset<T>(_Key);

	if (nullptr != Asset)
	{
		return Asset;
	}

	// 동일 키값의 에셋이 없었으면
	wstring strFilePath = DPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	Asset = new T;

	// 로딩 실패 시 예외처리
	if (FAILED(Asset->Load(strFilePath)))
	{
		MessageBox(nullptr, L"에셋 로딩 실패", L"로딩 실패", MB_OK);
		return nullptr;
	}

	// Asset 이 자신의 키값과 경로를 알게 함
	Asset->m_Key = _Key;
	Asset->m_RelativePath = _RelativePath;

	// 맵에 등록
	ASSET_TYPE type = GetAssetType<T>();
	m_mapAsset[(UINT)type].insert(make_pair(_Key, Asset.Get()));

	// Asset 변경 알림	
	DTaskMgr::GetInst()->AddTask(tTask{ ASSET_CHANGED });

	// 로딩된 에셋 주소 반환
	return Asset;
}

template<typename T>
Ptr<T> DAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<DAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	if (iter == m_mapAsset[(UINT)Type].end())
		return nullptr;

	//Ptr<DAsset> pAsset;
	//Ptr<DMesh> pMesh = (DMesh*)pAsset.Get()

	return (T*)iter->second.Get();
}

template<typename T>
void DAssetMgr::AddAsset(const wstring& _Key, Ptr<T> _Asset)
{
	ASSET_TYPE Type = GetAssetType<T>();

	assert(!FindAsset(Type, _Key).Get());

	_Asset->SetKey(_Key);
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, _Asset.Get()));

	// Asset 변경 알림	
	DTaskMgr::GetInst()->AddTask(tTask{ ASSET_CHANGED });
}

// File 에 Asset 참조정보 저장 불러오기
template<typename T>
void SaveAssetRef(Ptr<T> Asset, FILE* _File)
{
	bool bAsset = Asset.Get();
	fwrite(&bAsset, 1, 1, _File);

	if (bAsset)
	{
		SaveWString(Asset->GetKey(), _File);
		SaveWString(Asset->GetRelativePath(), _File);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& Asset, FILE* _File)
{
	bool bAsset = false;
	fread(&bAsset, 1, 1, _File);

	if (bAsset)
	{
		wstring key, relativepath;
		LoadWString(key, _File);
		LoadWString(relativepath, _File);

		Asset = DAssetMgr::GetInst()->Load<T>(key, relativepath);
	}
}

#endif // !ENGINE_DASSETMGR_H