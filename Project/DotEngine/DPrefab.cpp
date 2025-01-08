#include "pch.h"
#include "DPrefab.h"

// GameObject Headers
#include "DGameObject.h"

OBJECT_SAVE  DPrefab::g_ObjectSaveFunc = nullptr;
OBJECT_LOAD  DPrefab::g_ObjectLoadFunc = nullptr;

DPrefab::DPrefab()
    : DAsset(ASSET_TYPE::PREFAB)
    , m_ProtoObject(nullptr)
{
}

DPrefab::~DPrefab()
{
    if (nullptr != m_ProtoObject)
        delete m_ProtoObject;
}

DGameObject* DPrefab::Instantiate()
{
    return m_ProtoObject->Clone();
}

int DPrefab::Save(const wstring& _FilePath)
{
    FILE* File = nullptr;
    _wfopen_s(&File, _FilePath.c_str(), L"wb");

    if (nullptr == File)
        return E_FAIL;

    g_ObjectSaveFunc(File, m_ProtoObject);

    fclose(File);

    return S_OK;
}

int DPrefab::Load(const wstring& _FilePath)
{
    FILE* File = nullptr;
    _wfopen_s(&File, _FilePath.c_str(), L"rb");

    if (nullptr == File)
        return E_FAIL;

    m_ProtoObject = g_ObjectLoadFunc(File);

    fclose(File);

    return S_OK;
}
