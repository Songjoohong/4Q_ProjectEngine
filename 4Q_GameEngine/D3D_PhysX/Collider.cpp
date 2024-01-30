#include "pch.h"
#include "Collider.h"

Collider::Collider(Component::BoxCollider* owner)
{
	m_pOwner = owner;
	Initialize();
}

void Collider::Initialize()
{
	m_Scale = m_pOwner->m_Center;
	m_Transform.p.x = m_pOwner->m_Center.GetX();
	m_Transform.p.y = m_pOwner->m_Center.GetY();
	m_Transform.p.z = m_pOwner->m_Center.GetZ();
}
