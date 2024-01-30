#include "pch.h"
#include "Collider.h"

Collider::Collider(BoxCollider* owner)
{
	m_pOwner = owner;
	Initialize();
}

void Collider::Initialize()
{
	m_Scale = m_pOwner->mScale;
	m_Transform.p.x = m_pOwner->m_Center.x;
	m_Transform.p.y = m_pOwner->m_Center.y;
	m_Transform.p.z = m_pOwner->m_Center.z;
}
