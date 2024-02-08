#pragma once
#include "BoxCollider.h"

using namespace physx; 

/*
	석영 : modifypair 만들어야함
*/

class Collider
{
public:

	Collider(BoxCollider* owner);
	virtual ~Collider();

public:
	/*
		석영 : collider 종류에 따라서 초기화 해주기.
		Material 오브젝트 특성에 따라서 설정할 수 있도록. 
		기본값 {1.0f, 1.0f, 0.0f}
	*/
	virtual void Initialize();
	virtual void SetMaterial(float staticFriction, float DynamicFriction, float restitution) {}

	void UpdateTransform();
public:
	BoxCollider* m_pOwner;
	Vector3D m_Scale;

	PxRigidActor* m_pRigidActor=nullptr;
	PxTransform m_Transform;
	PxBoxGeometry m_BoxGeometry;
	PxMaterial* m_pMaterial;
	PxShape* m_pShape;

};

