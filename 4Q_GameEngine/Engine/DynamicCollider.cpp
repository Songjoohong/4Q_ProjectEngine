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
	// 석영 : Box만 사용중.
	m_pShape = PxRigidActorExt::createExclusiveShape(*m_Rigid, m_BoxGeometry, *m_pMaterial);
	m_pRigidActor = m_Rigid;
	PhysicsManager::GetInstance()->GetPxScene()->addActor(*m_pRigidActor);

	SetDensity(75.f); // 석영 : 기본값으로 넣어주기.
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

	m_Rigid->clearForce(PxForceMode::eIMPULSE);
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
	m_CurrentDir = direction;
	PxVec3 checkmove(0.f, 0.f, 0.f);

	PxVec3 dirTest;
	dirTest.x = m_CurrentDir.x * m_PrevDir.x;
	dirTest.y = m_CurrentDir.y * m_PrevDir.y;
	dirTest.z = m_CurrentDir.z * m_PrevDir.z;

	if (m_CurrentDir != checkmove)
	{
		if (dirTest.x<0|| dirTest.y < 0||dirTest.z<0)
		{
			m_Rigid->addForce(PxVec3(0.f, -1.f, 0.f) * m_Rigid->getMass()*1000.f, PxForceMode::eIMPULSE, true);
		}
		m_Rigid->addForce(m_CurrentDir * m_Rigid->getMass() * 10.f, PxForceMode::eIMPULSE, true);

		m_bKeyUp = false;
	}
	else if(m_CurrentDir==checkmove&& m_bKeyUp==false)
	{
		m_bKeyUp=true;
		m_Rigid->addForce(PxVec3(0.f, -1.f, 0.f) * m_Rigid->getMass()*1000.f, PxForceMode::eIMPULSE, true);
	}

	m_PrevDir = m_CurrentDir;
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
