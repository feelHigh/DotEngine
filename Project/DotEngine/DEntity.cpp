#include "pch.h"
#include "DEntity.h"

UINT DEntity::g_ID = 0;

DEntity::DEntity()
	: m_ID(g_ID++)
{
}

DEntity::DEntity(const DEntity& _Other)
	: m_ID(g_ID++)
	, m_Name(_Other.m_Name)
{
}

DEntity::~DEntity()
{
}
