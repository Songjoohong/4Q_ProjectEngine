#pragma once
#include "BoxCollider.h"
using namespace physx; 
using namespace Component; // 석영 : main 에서는 지워도 된다.

/*
	석영 : AddActor 해주기! 어떻게 해줄지..?
	PxTranform 위치로 부터 아래방향으로 콜라이더가 생성된다.
*/

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

