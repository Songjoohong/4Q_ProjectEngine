#include "pch.h"
#include "DynamicCollider.h"
#include "PhysicsManager.h"

DynamicCollider::DynamicCollider(BoxCollider* owner)
	:Collider(owner)
{
}

void DynamicCollider::Initialize()
{
	__super::Initialize();

	m_Rigid = PhysicsManager::GetInstance()->GetPhysics()->createRigidDynamic(m_Transform);
	// 석영 : Box만 사용중.
	m_pShape = PxRigidActorExt::createExclusiveShape(*m_Rigid, m_BoxGeometry, *m_pMaterial);
	m_pRigidActor = m_Rigid;
	PhysicsManager::GetInstance()->GetPxScene()->addActor(*m_pRigidActor);

	SetDensity(60.f); // 석영 : 기본값으로 넣어주기.
}

void DynamicCollider::UpdatePhysics()
{
	/*
		석영 : Owner 위치,회전값 변경해주기. 
	*/
   
	PxTransform pxTrans = m_Rigid->getGlobalPose();
	m_pOwner->m_Center.SetX(pxTrans.p.x);
	m_pOwner->m_Center.SetY(pxTrans.p.y);
	m_pOwner->m_Center.SetZ(pxTrans.p.z);

	PxReal angle = 180.f / PxPi;
	m_pOwner->m_Rotation.SetX(pxTrans.q.x * angle);
	m_pOwner->m_Rotation.SetY(pxTrans.q.y * angle);
	m_pOwner->m_Rotation.SetZ(pxTrans.q.z * angle);
}

void DynamicCollider::SetDensity(float mass)
{
	float density = mass / (m_Scale.GetX() * m_Scale.GetY() * m_Scale.GetZ());
	PxRigidBodyExt::updateMassAndInertia(*m_Rigid, density);
}

void DynamicCollider::AddForce(Vector3D dir)
{
	PxVec3 direction(dir.GetX(), dir.GetY(), dir.GetZ());
	m_Rigid->addForce(direction*100000.f,PxForceMode::eFORCE,true);
}

void DynamicCollider::FreezeRotationX(bool active)
{
	m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, active);
}

void DynamicCollider::FreezeRotationY(bool active)
{
	m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, active);
}

void DynamicCollider::FreezeRotationZ(bool active)
{
	m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, active);
}
