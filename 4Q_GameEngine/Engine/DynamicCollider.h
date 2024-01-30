#pragma once
#include "Collider.h"

class DynamicCollider
	:public Collider
{
public:
	DynamicCollider(BoxCollider* owner);
	~DynamicCollider();

public:
	/*
		���� :
		�浹ó��
		��ġ ������Ʈ�� ������� �Ѵ�.
		������Ʈ���� ��ġ���� �����ؾ��Ѵ�.
	*/
	void UpdatePhysics();
	void UpdateTransform();
public:
	PxRigidDynamic* m_Rigid;
};

