#include "pch.h"
#include "StaticCollider.h"

StaticCollider::StaticCollider(BoxCollider* owner)
	:Collider(owner)
{
}

void StaticCollider::Initailize()
{
	/*
	m_Rigid = PxCreateStatic(
		*m_pPhys->m_pPhysics,
		PxTransform(PxVec3(m_ObjectTransform.x, -300.f, 0)),
		PxBoxGeometry(1000.f, 2.f, 1000.f),
		*m_pPhys->m_pMaterial
	);
	*/
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
