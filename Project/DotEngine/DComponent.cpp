#include "pch.h"
#include "DComponent.h"

DComponent::DComponent(COMPONENT_TYPE _Type)
	: m_Type(_Type)
	, m_Owner(nullptr)
{
}

DComponent::DComponent(const DComponent& _Origin)
	: m_Type(_Origin.m_Type)
	, m_Owner(nullptr)
{
}

DComponent::~DComponent()
{
}

void DComponent::Begin()
{
}

void DComponent::Tick()
{
}
