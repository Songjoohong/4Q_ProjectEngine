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

	m_Rigid = PhysicsManager::GetInstance()->GetPhysics()->createRigidDynamic(PxTransform(m_Transform.p));
	m_Rigid->setMaxLinearVelocity(100.f);
	// ���� : Box�� �����.
	m_pShape = PxRigidActorExt::createExclusiveShape(*m_Rigid, m_BoxGeometry, *m_pMaterial);
	m_pRigidActor = m_Rigid;
	PhysicsManager::GetInstance()->GetPxScene()->addActor(*m_pRigidActor);

	SetDensity(75.f); // ���� : �⺻������ �־��ֱ�.
	SetFilterData();
}

void DynamicCollider::SetFilterData()
{
	PxFilterData* filter=PhysicsManager::GetInstance()->GetFilterData(m_pOwner->m_CollisionMask);
	assert(filter != nullptr);
	m_pShape->setSimulationFilterData(*filter);
	PxU32 filterdata = filter->word0;
	m_Rigid->userData = (void*)filter;
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
	/*
		���� : �������� ���� ������ �� ���� �� ����.
	*/
  	PxVec3 direction(dir.GetX(), dir.GetY(), dir.GetZ());
	PxVec3 checkmove(0.f, 0.f, 0.f);
	m_CurrentDir.x = dir.GetX();
	m_CurrentDir.y = dir.GetY();
	m_CurrentDir.z = dir.GetZ();
	if (m_CurrentDir != checkmove)
	{
		m_Rigid->addForce(m_CurrentDir * m_Rigid->getMass() * 2.f, PxForceMode::eIMPULSE, true);
		m_bKeyUp = false;
	}
	else if(m_CurrentDir==checkmove&& m_bKeyUp==false)
	{
		m_bKeyUp=true;
		m_Rigid->addForce(PxVec3(0.f, -1.f, 0.f) * m_Rigid->getMass()*10.f, PxForceMode::eIMPULSE, true);
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

