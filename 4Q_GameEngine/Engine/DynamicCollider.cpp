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

	m_Rigid = PxCreateDynamic(
		*(PhysicsManager::GetInstance()->m_pPhysics),
		m_Transform,
		m_BoxGeometry,
		*m_pMaterial, 
		10.f);

	SetMass(0.3f); // 석영 : 기본값으로 넣어주기.
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

	m_pOwner->m_Rotation.SetX(pxTrans.q.x);
	m_pOwner->m_Rotation.SetY(pxTrans.q.y);
	m_pOwner->m_Rotation.SetZ(pxTrans.q.z);
}

void DynamicCollider::UpdateTransform()
{
	/*
		석영 : Owner 위치에 따라서 collider 위치 변경 해주기.
	*/

	m_Transform.p.x=m_pOwner->m_Center.GetX();
	m_Transform.p.y=m_pOwner->m_Center.GetY();
	m_Transform.p.z=m_pOwner->m_Center.GetZ();

	m_Transform.q.x = m_pOwner->m_Rotation.GetX();
	m_Transform.q.y = m_pOwner->m_Rotation.GetY();
	m_Transform.q.z = m_pOwner->m_Rotation.GetZ();
}

void DynamicCollider::SetMass(float mass)
{
	m_Rigid->setMass(mass);
	PxRigidBodyExt::updateMassAndInertia(*m_Rigid, mass);
}

void DynamicCollider::AddForce(Vector3D dir)
{
	PxVec3 direction(dir.GetX(), dir.GetY(), dir.GetZ());
	m_Rigid->addForce(direction);
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
