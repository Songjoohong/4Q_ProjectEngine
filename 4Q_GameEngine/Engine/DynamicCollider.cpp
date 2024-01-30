#include "pch.h"
#include "DynamicCollider.h"

DynamicCollider::DynamicCollider(BoxCollider* owner)
	:Collider(owner)
{
}

void DynamicCollider::UpdatePhysics()
{
	/*
		���� : Owner ��ġ �������ֱ�. 
		��ȫ�� ���� ����� -> BoxCollider ��ġ �����ϸ� ������Ʈ ��ġ������ ����?
	*/

	PxTransform pxTrans = m_Rigid->getGlobalPose();
	m_pOwner->m_Center.SetX(pxTrans.p.x);
	m_pOwner->m_Center.SetY(pxTrans.p.y);
	m_pOwner->m_Center.SetZ(pxTrans.p.z);
}

void DynamicCollider::UpdateTransform()
{
	/*
		���� : Owner ��ġ�� ���� collider ��ġ ���� ���ֱ�.
	*/

	m_Transform.p.x=m_pOwner->m_Center.GetX();
	m_Transform.p.y=m_pOwner->m_Center.GetY();
	m_Transform.p.z=m_pOwner->m_Center.GetZ();
}
