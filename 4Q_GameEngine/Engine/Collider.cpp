#include "pch.h"
#include "Collider.h"

Collider::Collider(BoxCollider* owner)
{
	m_pOwner = owner;
	Initialize();
}

void Collider::Initialize()
{
	m_Scale = m_pOwner->m_Size;
	m_BoxGeometry = { m_Scale.GetX() / 2,m_Scale.GetY() / 2,m_Scale.GetZ() / 2 };

	// 석영 : 이거는 나중에 업데이트 돌려주는걸로 바꿀 수 있으면 바꾸기.
	m_Transform.p.x = m_pOwner->m_Center.GetX();
	m_Transform.p.y = m_pOwner->m_Center.GetY();
	m_Transform.p.z = m_pOwner->m_Center.GetZ();
}