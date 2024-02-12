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
	SetFilterData();
}

void DynamicCollider::SetFilterData()
{
	PxFilterData* filter = PhysicsManager::GetInstance()->GetFilterData(m_pOwner->m_CollisionMask);
	assert(filter != nullptr);
	m_pShape->setSimulationFilterData(*filter);
	PxU32 filterdata = filter->word0;
	m_Rigid->userData = (void*)filterdata;
}

void DynamicCollider::UpdatePhysics()
{
	/*
		석영 : Owner 위치,회전값 변경해주기.
	*/

	PxTransform pxTrans = m_Rigid->getGlobalPose();
	m_Transform = pxTrans;
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
		석영 : 떨어지는 동안 움직일 수 없게 할 예정.
	*/
	PxVec3 direction(dir.GetX(), dir.GetY(), dir.GetZ());
	PxVec3 checkmove(0.f, 0.f, 0.f);
	m_CurrentDir.x = dir.GetX();
	m_CurrentDir.y = dir.GetY();
	m_CurrentDir.z = dir.GetZ();

	CheckTerrain();

	if (m_CurrentDir != checkmove)
	{
		if (m_MoveStairs)
		{
			PxVec3 curPos = m_Rigid->getGlobalPose().p;
			m_Rigid->setGlobalPose(PxTransform(curPos.x, curPos.y + m_CurrentDir.y/60.f, curPos.z + m_CurrentDir.z));
		}
		else
			m_Rigid->addForce(m_CurrentDir * m_Rigid->getMass() * 2.f, PxForceMode::eIMPULSE, true);

		m_bKeyUp = false;
	}
	else if (m_CurrentDir == checkmove && m_bKeyUp == false)
	{
		m_bKeyUp = true;
		m_Rigid->addForce(PxVec3(0.f, -1.f, 0.f) * m_Rigid->getMass() * 10.f, PxForceMode::eIMPULSE, true);
	}

	m_PrevDir = m_CurrentDir;
}

void DynamicCollider::CheckTerrain()
{
	PxVec3 raycastPos = m_Transform.p;
	//raycastPos.y -= 50.f;
	raycastPos.z -= m_Scale.GetZ() / 2.f;
	m_IsGround = PhysicsManager::GetInstance()->CheckGround(raycastPos);
	//m_IsStairs = PhysicsManager::GetInstance()->CheckStairs(raycastPos);
	m_IsStairs = PhysicsManager::GetInstance()->GetState();

	if (m_IsGround && !m_IsStairs)
	{
		m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, true);
	}
	else if (!m_IsGround && m_IsStairs)
		m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, false);
	else if (!m_IsGround)
	{
		m_Rigid->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, false);
	}

	if (m_IsStairs)
	{
		if (m_CurrentDir.z < 0)
		{
			m_MoveStairs = true;
			m_CurrentDir.y = 1.f;
		}
		else if (m_CurrentDir.z > 0)
		{
			m_MoveStairs = true;
			m_CurrentDir.y = -1.f;
		}
		else
		{
			m_MoveStairs = false;
		}
	}
	else
		m_MoveStairs = false;

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

