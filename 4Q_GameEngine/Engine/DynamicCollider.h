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
		석영 :
		충돌처리
		위치 업데이트를 시켜줘야 한다.
		오브젝트에게 위치값을 전달해야한다.
	*/
	void UpdatePhysics();
	void UpdateTransform();
public:
	PxRigidDynamic* m_Rigid;
};

