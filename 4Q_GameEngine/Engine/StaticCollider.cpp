#include "pch.h"
#include "StaticCollider.h"

#include "PhysicsManager.h"

StaticCollider::StaticCollider(BoxCollider* owner)
	:Collider(owner)
{
}

StaticCollider::~StaticCollider()
{
	delete static_cast<UserData*>(m_Rigid->userData);
	PX_RELEASE(m_Rigid);
}

void StaticCollider::Initialize()
{
	__super::Initialize();

	m_Rigid = PhysicsManager::GetInstance()->GetPhysics()->createRigidStatic(PxTransform(m_Transform.p));

	// 석영 : shape 현재 Box만 사용.
	m_pShape = PxRigidActorExt::createExclusiveShape(*m_Rigid, m_BoxGeometry, *m_pMaterial);
	if (m_pOwner->m_CollisionType == CollisionType::TRIGGER || m_pOwner->m_CollisionType == CollisionType::ROOM)
	{
		m_pShape->setFlags(PxShapeFlag::eTRIGGER_SHAPE);
	}

	//PhysicsManager::GetInstance()->GetPxScene()->addActor(*m_Rigid);
	PhysicsManager::GetInstance()->AddStaticColliders(this);

	SetFilterData();
	UpdatePosition();
}

void StaticCollider::UpdatePosition()
{
	PxVec3 boxCenter =
	{
		m_pOwner->m_Center.GetX(),
		m_pOwner->m_Center.GetY(),
		m_pOwner->m_Center.GetZ()
	};

	PxVec3 boxPos =
	{
		m_pOwner->m_WorldPosition.GetX(),
		m_pOwner->m_WorldPosition.GetY(),
		m_pOwner->m_WorldPosition.GetZ()
	};

	if (boxPos != m_Transform.p)
	{
		m_Transform.p = boxPos;
		m_pOwner->m_WorldPosition = { m_Transform.p.x,m_Transform.p.y,m_Transform.p.z };
		m_Rigid->setGlobalPose(m_Transform);
	}
}
