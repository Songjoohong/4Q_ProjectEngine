#pragma once
#include "..\Engine\Vector3D.h"
#include "Collider.h"

using namespace physx;

struct BoxCollider;
class DynamicCollider
	: public Collider
{
public:
	DynamicCollider(BoxCollider* owner);
	virtual ~DynamicCollider()=default;

public:
	/*
		���� :
		�浹ó�� 
		��ġ ������Ʈ�� ������� �Ѵ�.
		������Ʈ���� ��ġ���� �����ؾ��Ѵ�.
	*/
	void Update();
	
public:
	PxRigidDynamic*  m_Rigid;
};	

