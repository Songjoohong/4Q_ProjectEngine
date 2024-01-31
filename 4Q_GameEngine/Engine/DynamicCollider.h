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

	virtual void Initialize();

	void UpdatePhysics();
	void UpdateTransform();

	// 석영 : 물리 효과에서 회전축 고정용
	void FreezeRotationX(bool active);
	void FreezeRotationY(bool active);
	void FreezeRotationZ(bool active);
public:
	PxRigidDynamic* m_Rigid;
};

