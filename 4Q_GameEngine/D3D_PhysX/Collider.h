#pragma once
using namespace physx;
/*
	Todo : ����
	Engine ���� BoxCollider�� ������ �� �Ѱܹ޾Ƽ� �ȿ� �ִ� ���� ������ ���������ϱ�.
*/

struct BoxCollider;
class Vector3D;
class Collider
{
public:
	Collider(BoxCollider* owner);
	~Collider()=default;

public:
	/*
		���� : collider ������ ���� �ʱ�ȭ ���ֱ�.
	*/
	virtual void Initialize(); 

public:
	BoxCollider* m_pOwner;
	Vector3D m_Scale;

	PxShape* m_pShape;
	PxTransform m_Transform;
	PxBoxGeometry m_BoxGeoMetry;
};

