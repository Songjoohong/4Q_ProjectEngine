#include "pch.h"
#include "DynamicCollider.h"
#include "PhysicsManager.h"

DynamicCollider::DynamicCollider(BoxCollider* owner)
	:Collider(owner)
{
}

DynamicCollider::~DynamicCollider()
{
	delete static_cast<UserData*>(m_Rigid->userData);
	PX_RELEASE(m_Rigid);
}

void DynamicCollider::Initialize()
{
	__super::Initialize();

	m_Rigid = PhysicsManager::GetInstance()->GetPhysics()->createRigidDynamic(PxTransform(m_Transform.p));
	// 석영 : Box만 사용중.
	m_Rigid->setMaxLinearVelocity(150.f);
	m_pShape = PxRigidActorExt::createExclusiveShape(*m_Rigid, m_BoxGeometry, *m_pMaterial);
	//PhysicsManager::GetInstance()->GetPxScene()->addActor(*m_Rigid);

	PhysicsManager::GetInstance()->AddDynamicColliders(this);
	
	FreezeRotation(true, true, true);
	SetDensity(75.f); // 석영 : 기본값으로 넣어주기.
	SetFilterData();
	UpdatePosition();
}
void DynamicCollider::UpdatePosition()
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
		m_pOwner->m_WorldPosition.GetY() + m_Scale.GetY() / 2.f ,
		m_pOwner->m_WorldPosition.GetZ()
	};

	if (boxPos != m_Transform.p)
	{
		m_Transform.p = boxPos;
		m_Transform.p.y += m_Scale.GetY() / 2.f;
		m_pOwner->m_WorldPosition = { m_Transform.p.x,m_Transform.p.y,m_Transform.p.z };
		m_Rigid->setGlobalPose(m_Transform);
	}
}


void DynamicCollider::UpdatePhysics()
{
	/*
		석영 : Owner 위치,회전값 변경해주기.
	*/

	PxTransform pxTrans = m_Rigid->getGlobalPose();
	m_Transform = pxTrans;
	m_pOwner->m_WorldPosition = {
		m_Transform.p.x,
		m_Transform.p.y,
		m_Transform.p.z
	};

	m_Transform.p.y += m_Scale.GetY() / 2.f;
}

void DynamicCollider::SetDensity(float mass)
{
	float density = mass / (m_Scale.GetX() * m_Scale.GetY() * m_Scale.GetZ());
	PxRigidBodyExt::updateMassAndInertia(*m_Rigid, density);

}

void DynamicCollider::AddForce(Vector3D dir)
{
	/*
		석영 : 떨어지는 동안 움직일 수 없게 할 예정.
	*/
	PxVec3 direction(dir.GetX(), dir.GetY(), dir.GetZ());
	PxVec3 checkmove(0.f, 0.f, 0.f);
	m_CurrentDir.x = dir.GetX();
	m_CurrentDir.y = dir.GetY();
	m_CurrentDir.z = dir.GetZ();

	if (m_CurrentDir != checkmove)
	{
		float mass = m_Rigid->getMass();
		m_Rigid->addForce(m_CurrentDir * m_Rigid->getMass() * 2.f, PxForceMode::eIMPULSE, true);
		m_bKeyUp = false;
	}
	else if (m_CurrentDir == checkmove && m_bKeyUp == false)
	{
		m_bKeyUp = true;
		m_Rigid->addForce(PxVec3(0.f, -1.f, 0.f) * m_Rigid->getMass() * 150.f, PxForceMode::eIMPULSE, true);
	}

	m_PrevDir = m_CurrentDir;
}

void DynamicCollider::FreezeRotation(bool x_active, bool y_active, bool z_active)
{
	m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, x_active);
	m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, y_active);
	m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, z_active);
}

void DynamicCollider::FreezeLinear(bool x_active, bool y_active, bool z_active)
{
	m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, x_active);
	m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, y_active);
	m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, z_active);
}

