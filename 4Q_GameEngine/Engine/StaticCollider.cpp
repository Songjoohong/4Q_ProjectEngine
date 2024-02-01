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
