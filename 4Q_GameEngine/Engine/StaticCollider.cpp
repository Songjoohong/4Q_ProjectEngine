#include "pch.h"
#include "StaticCollider.h"

#include "PhysicsManager.h"

StaticCollider::StaticCollider(BoxCollider* owner)
	:Collider(owner)
{
}

void StaticCollider::Initialize()
{
	__super::Initialize();

	m_Rigid = PhysicsManager::GetInstance()->GetPhysics()->createRigidStatic(PxTransform(m_Transform.p));

	// 석영 : shape 현재 Box만 사용.
	//Test
	m_pShape = PxRigidActorExt::createExclusiveShape(*m_Rigid, m_BoxGeometry, *m_pMaterial);

	if (m_pOwner->m_CollisionMask == Collision_Mask::SLOPE)
	{
		float degrees = 45.f;
		float radians = PxDegToRad(degrees);
		PxQuat rotation(radians, PxVec3(-1.0f, 0.0f, 0.0f));
		PxTransform pos;
		pos = { m_Transform.p.x,m_Transform.p.y,m_Transform.p.x };
		pos.q = rotation;

		m_pShape->setLocalPose(pos);
	}
	m_pRigidActor = m_Rigid;
	PhysicsManager::GetInstance()->GetPxScene()->addActor(*m_pRigidActor);
	SetFilterData();
}

void StaticCollider::SetFilterData()
{
	PxFilterData* filter = PhysicsManager::GetInstance()->GetFilterData(m_pOwner->m_CollisionMask);
	assert(filter != nullptr);
	m_pShape->setSimulationFilterData(*filter);
	PxU32 filterdata = filter->word0;
	m_Rigid->userData = (void*)filterdata;

}
