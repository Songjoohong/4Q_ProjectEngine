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
	m_pMaterial = PhysicsManager::GetInstance()->GetPhysics()->createMaterial(0.5f, 0.5f, 0.0f);

	UpdateTransform();
}


void Collider::UpdateTransform()
{
	/*
		석영 : BoxCollider 위치에 따라서 옮겨주기
	*/

	m_Transform.p.x = m_pOwner->m_Center.GetX();
	m_Transform.p.y = m_pOwner->m_Center.GetY() + m_pOwner->m_Size.GetY();
	m_Transform.p.z = m_pOwner->m_Center.GetZ();

	PxReal angle = PxPi / 180.f;
	m_Transform.q.x = m_pOwner->m_Rotation.GetX() * angle;
	m_Transform.q.y = m_pOwner->m_Rotation.GetY() * angle;
	m_Transform.q.z = m_pOwner->m_Rotation.GetZ() * angle;
}

//void Collider::SetObjectType(ObjectType type)
//{
//	m_ObjectType = type;
//	/*
//		석영 : 오브젝트 종류별 세팅도 같이 해주기.
//	*/
//
//	m_pShape->setSimulationFilterData(*(PhysicsManager::GetInstance()->GetFilterData(type)));
//	m_pRigidActor->userData = (PhysicsManager::GetInstance()->GetUserData(type));
//}