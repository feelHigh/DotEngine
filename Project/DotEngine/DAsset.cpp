#include "pch.h"
#include "DAsset.h"

DAsset::DAsset(ASSET_TYPE _Type)
	: m_Type(_Type)
	, m_RefCount(0)
	, m_Engine(false)
{
}

DAsset::DAsset(const DAsset& _Other)
	: DEntity(_Other)
	, m_Key(_Other.m_Key)
	, m_RelativePath(_Other.m_RelativePath)
	, m_Type(_Other.m_Type)
	, m_RefCount(0)
{
}

DAsset::~DAsset()
{
}
