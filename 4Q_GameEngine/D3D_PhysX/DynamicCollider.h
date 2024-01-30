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
		석영 :
		충돌처리 
		위치 업데이트를 시켜줘야 한다.
		오브젝트에게 위치값을 전달해야한다.
	*/
	void Update();
	
public:
	PxRigidDynamic*  m_Rigid;
};	

