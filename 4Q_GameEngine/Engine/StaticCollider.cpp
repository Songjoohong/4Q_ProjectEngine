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
	m_pShape = PxRigidActorExt::createExclusiveShape(*m_Rigid, m_BoxGeometry, *m_pMaterial);
	m_pRigidActor = m_Rigid;
	PhysicsManager::GetInstance()->GetPxScene()->addActor(*m_pRigidActor);
}

void StaticCollider::SetFiltercData()
{
	PxFilterData* filter = PhysicsManager::GetInstance()->GetFilterData(m_pOwner->m_CollisionType);
	assert(filter != nullptr);
	m_pShape->setSimulationFilterData(*filter);
	PxU32 filterdata = filter->word0;

	UserData* data = new UserData;
	data->m_CollisionType = m_pOwner->m_CollisionType;
}
