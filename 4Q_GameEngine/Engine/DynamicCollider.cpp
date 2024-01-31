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
		*(PhysicsManager::GetInstance()->m_pMaterial),
		10.f);
}

void DynamicCollider::UpdatePhysics()
{
	/*
		���� : Owner ��ġ,ȸ���� �������ֱ�. 
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
		���� : Owner ��ġ�� ���� collider ��ġ ���� ���ֱ�.
	*/

	m_Transform.p.x=m_pOwner->m_Center.GetX();
	m_Transform.p.y=m_pOwner->m_Center.GetY();
	m_Transform.p.z=m_pOwner->m_Center.GetZ();

	m_Transform.q.x = m_pOwner->m_Rotation.GetX();
	m_Transform.q.y = m_pOwner->m_Rotation.GetY();
	m_Transform.q.z = m_pOwner->m_Rotation.GetZ();
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
