#include "pch.h"
#include "DynamicCollider.h"

DynamicCollider::DynamicCollider(BoxCollider* owner)
	:Collider(owner)
{
}

void DynamicCollider::UpdatePhysics()
{
	/*
		석영 : Owner 위치 변경해주기. 
		주홍이 에게 물어보기 -> BoxCollider 위치 변경하면 오브젝트 위치변경이 가능?
	*/

	PxTransform pxTrans = m_Rigid->getGlobalPose();
	m_pOwner->m_Center.SetX(pxTrans.p.x);
	m_pOwner->m_Center.SetY(pxTrans.p.y);
	m_pOwner->m_Center.SetZ(pxTrans.p.z);
}

void DynamicCollider::UpdateTransform()
{
	/*
		석영 : Owner 위치에 따라서 collider 위치 변경 해주기.
	*/

	m_Transform.p.x=m_pOwner->m_Center.GetX();
	m_Transform.p.y=m_pOwner->m_Center.GetY();
	m_Transform.p.z=m_pOwner->m_Center.GetZ();
}
