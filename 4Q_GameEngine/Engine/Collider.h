#pragma once
#include "BoxCollider.h"
using namespace physx; 
using namespace Component; // ���� : main ������ ������ �ȴ�.

class Collider
{
public:

	Collider(BoxCollider* owner);
	virtual ~Collider() = default;

public:
	/*
		���� : collider ������ ���� �ʱ�ȭ ���ֱ�.
	*/
	virtual void Initialize();

public:
	BoxCollider* m_pOwner;
	Vector3D m_Scale;

	PxTransform m_Transform;
	PxBoxGeometry m_BoxGeometry;
};

