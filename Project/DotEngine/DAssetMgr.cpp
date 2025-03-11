#include "pch.h"
#include "DAssetMgr.h"

// Base Headers
#include "AssetList.h"

DAssetMgr::DAssetMgr()
{
}

DAssetMgr::~DAssetMgr()
{
}

void DAssetMgr::Tick()
{
	if (m_Changed)
		m_Changed = false;
}

Ptr<DAsset> DAssetMgr::FindAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	map<wstring, Ptr<DAsset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_Key);

	if (iter == m_mapAsset[(UINT)_Type].end())
	{
		return nullptr;
	}

	return iter->second;
}

Ptr<DTexture> DAssetMgr::CreateTexture(wstring _strKey, UINT _Width, UINT _Height
	, DXGI_FORMAT _Format, UINT _Flags, D3D11_USAGE _Usage)
{
	// Duplicate key check
	Ptr<DTexture> pTexture = FindAsset<DTexture>(_strKey);
	assert(!pTexture.Get());

	pTexture = new DTexture;
	if (FAILED(pTexture->Create(_Width, _Height, _Format, _Flags, _Usage)))
	{
		MessageBox(nullptr, L"Failed loading texture.(DAssetMgr)", L"Failed loading texture.(DAssetMgr)", MB_OK);
		return nullptr;
	}

	pTexture->m_Key = _strKey;
	pTexture->SetEngineAsset();
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_strKey, pTexture.Get()));

	return pTexture;
}

Ptr<DTexture> DAssetMgr::CreateTexture(wstring _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	// Duplicate key check
	Ptr<DTexture> pTexture = FindAsset<DTexture>(_strKey);
	assert(!pTexture.Get());

	pTexture = new DTexture;
	if (FAILED(pTexture->Create(_Tex2D)))
	{
		MessageBox(nullptr, L"Failed loading texture.(DAssetMgr)", L"Failed loading texture.(DAssetMgr)", MB_OK);
		return nullptr;
	}

	pTexture->m_Key = _strKey;
	pTexture->SetEngineAsset();
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_strKey, pTexture.Get()));

	return pTexture;
}

void DAssetMgr::GetAssetNames(ASSET_TYPE _Type, vector<string>& _vecOut)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		_vecOut.push_back(string(pair.first.begin(), pair.first.end()));
	}
}

void DAssetMgr::DeleteAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	map<wstring, Ptr<DAsset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_Key);
	assert(iter != m_mapAsset[(UINT)_Type].end());
	m_mapAsset[(UINT)_Type].erase(iter);

	// Asset Change Notification
	DTaskMgr::GetInst()->AddTask(tTask{ ASSET_CHANGED });
}
