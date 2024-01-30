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
		static �� �浹ó���� ���� �����൵ �Ǳ⿡ ������Ʈx
	*/

public:
	PxRigidStatic*	 m_Rigid;
};

