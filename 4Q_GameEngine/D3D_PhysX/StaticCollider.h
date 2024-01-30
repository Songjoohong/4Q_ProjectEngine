#pragma once
#include "Collider.h"

using namespace physx;

struct BoxCollider;
class StaticCollider
	:public Collider
{
public: 
	StaticCollider(BoxCollider* owner);
	~StaticCollider() = default;

public:
	/*
		static 은 충돌처리를 따로 안해줘도 되기에 업데이트x
	*/

public:
	PxRigidStatic*	 m_Rigid;
};

