#pragma once
#include "BoxCollider.h"
using namespace physx; 
using namespace Component; // 석영 : main 에서는 지워도 된다.

class Collider
{
public:

	Collider(BoxCollider* owner);
	virtual ~Collider() = default;

public:
	/*
		석영 : collider 종류에 따라서 초기화 해주기.
	*/
	virtual void Initialize();

public:
	BoxCollider* m_pOwner;
	Vector3D m_Scale;

	PxTransform m_Transform;
	PxBoxGeometry m_BoxGeometry;
};

