#pragma once
using namespace physx;
/*
	Todo : 석영
	Engine 에서 BoxCollider가 생성될 때 넘겨받아서 안에 있는 값을 가지고 이케저케하기.
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
		석영 : collider 종류에 따라서 초기화 해주기.
	*/
	virtual void Initialize(); 

public:
	BoxCollider* m_pOwner;
	Vector3D m_Scale;

	PxShape* m_pShape;
	PxTransform m_Transform;
	PxBoxGeometry m_BoxGeoMetry;
};

