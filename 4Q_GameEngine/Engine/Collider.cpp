#include "pch.h"
#include "Collider.h"
#include "PhysicsManager.h"

Collider::Collider(BoxCollider* owner)
{
	m_pOwner = owner;
}

Collider::~Collider()
{
}

void Collider::Initialize()
{
	m_Scale = m_pOwner->m_Size;
	m_BoxGeometry = { m_Scale.GetX() / 2,m_Scale.GetY() / 2,m_Scale.GetZ() / 2 };
	m_pMaterial = PhysicsManager::GetInstance()->GetPhysics()->createMaterial(1.0f, 1.0f, 0.0f);

	m_Transform = { m_pOwner->m_Center.GetX(),m_pOwner->m_Center.GetY() ,m_pOwner->m_Center.GetZ() };
}

void Collider::SetFilterData()
{
	PxFilterData* filter = PhysicsManager::GetInstance()->GetFilterData(m_pOwner->m_CollisionType);
	assert(filter != nullptr);
	m_pShape->setSimulationFilterData(*filter);
}

void Collider::UpdateRotation()
{
	/*
		석영 : 회전 바꿔주기
	*/
	PxVec3 deg;
	deg.x = PxDegToRad(m_pOwner->m_Rotation.GetX());
	deg.y = PxDegToRad(m_pOwner->m_Rotation.GetY());
	deg.z = PxDegToRad(m_pOwner->m_Rotation.GetZ());
	PxQuat angleX = PxQuat(deg.x, PxVec3(1.0f, 0.0f, 0.0f));
	PxQuat angleY = PxQuat(deg.y, PxVec3(0.0f, 1.0f, 0.0f));
	PxQuat angleZ = PxQuat(deg.z, PxVec3(0.0f, 0.0f, 1.0f));
	PxTransform newAngle;
	newAngle.p = { 0,0,0 };
	newAngle.q= angleX * angleY * angleZ;
	//m_Transform.q = angleX * angleY * angleZ;
	m_pShape->setLocalPose(newAngle);
}

void Collider::UpdateScale()
{
	if (m_Scale != m_pOwner->m_Size)
	{
		m_Scale = m_pOwner->m_Size;
		m_BoxGeometry = { m_Scale.GetX() / 2,m_Scale.GetY() / 2,m_Scale.GetZ() / 2 };
		m_pShape->setGeometry(m_BoxGeometry);
	}
}
