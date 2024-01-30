#pragma once
using namespace physx;
/*
	Todo : ����
	Engine ���� BoxCollider�� ������ �� �Ѱܹ޾Ƽ� �ȿ� �ִ� ���� ������ ���������ϱ�.
*/
#include "../Engine/BoxCollider.h"
class Vector3D;
class Collider
{
public:
	Collider(Component::BoxCollider* owner);
	virtual ~Collider() = default;

public:
	/*
		���� : collider ������ ���� �ʱ�ȭ ���ֱ�.
	*/
	virtual void Initialize(); 

public:
	Component::BoxCollider* m_pOwner;
	Vector3D m_Scale;

	PxShape* m_pShape;
	PxTransform m_Transform;
	PxBoxGeometry m_BoxGeometry;
};

