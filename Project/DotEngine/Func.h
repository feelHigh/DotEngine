#pragma once

void CreateObject(class DGameObject* _NewObject, int _LayerIndex);
void DeleteObject(DGameObject* _DeleteObject);
void ChangeLevelState(LEVEL_STATE _NextState);
void ChangeLevel(class DLevel* _Level, LEVEL_STATE _NextLevelState);

bool IsValid(DGameObject*& _Object);

void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Life, bool _DepthTest);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Life, bool _DepthTest);

void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Life, bool _DepthTes);


void SaveWString(const wstring& _String, FILE* _File);
void LoadWString(wstring& _String, FILE* _File);


template<typename T, int _Size>
void Delete_Array(T(&_arr)[_Size])
{
	for (int i = 0; i < _Size; ++i)
	{
		if (nullptr != _arr[i])
			delete _arr[i];
	}
}

template<typename T>
void Delete_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Delete_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (nullptr != pair.second)
			delete pair.second;
	}

	_map.clear();
}

#include "AssetList.h"
template<typename T>
ASSET_TYPE GetAssetType()
{
	if constexpr (std::is_same_v<T, DMesh>)
		return ASSET_TYPE::MESH;
	if constexpr (std::is_same_v<T, DMaterial>)
		return ASSET_TYPE::MATERIAL;
	if constexpr (std::is_same_v<T, DTexture>)
		return ASSET_TYPE::TEXTURE;
	if constexpr (std::is_same_v<T, DSprite>)
		return ASSET_TYPE::SPRITE;
	if constexpr (std::is_same_v<T, DFlipbook>)
		return ASSET_TYPE::FLIPBOOK;
	if constexpr (std::is_same_v<T, DSound>)
		return ASSET_TYPE::SOUND;
	if constexpr (std::is_same_v<T, DPrefab>)
		return ASSET_TYPE::PREFAB;
	if constexpr (std::is_same_v<T, DGraphicShader>)
		return ASSET_TYPE::GRAPHIC_SHADER;
	if constexpr (std::is_same_v<T, DComputeShader>)
		return ASSET_TYPE::COMPUTE_SHADER;
}