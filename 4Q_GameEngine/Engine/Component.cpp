#include "pch.h"
#include "Component.h"

void Component::SetOwner(GameObject* owner)
{
	m_pOwner = owner;
}

void Component::SetCode(GameObject* owner) const
{
	owner->m_ComponentCode += m_Code;
}

