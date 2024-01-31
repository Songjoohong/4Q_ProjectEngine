#include "pch.h"
#include "StaticCollider.h"

#include "PhysicsManager.h"

StaticCollider::StaticCollider(BoxCollider* owner)
	:Collider(owner)
{
}

void StaticCollider::Initailize()
{
	__super::Initialize();

	m_Rigid = PxCreateStatic(
		*(PhysicsManager::GetInstance()->m_pPhysics),
		m_Transform,
		m_BoxGeometry,
		*m_pMaterial);
}

void StaticCollider::UpdateTransform()
{
	/*
		���� : Owner ��ġ�� ���� collider ��ġ ���� ���ֱ�.
	*/

	m_Transform.p.x = m_pOwner->m_Center.GetX();
	m_Transform.p.y = m_pOwner->m_Center.GetY();
	m_Transform.p.z = m_pOwner->m_Center.GetZ();
}
