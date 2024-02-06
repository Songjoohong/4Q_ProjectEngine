#pragma once
#include "BoxCollider.h"

using namespace physx; 

/*
	���� : modifypair ��������
*/

class Collider
{
public:

	Collider(BoxCollider* owner);
	virtual ~Collider();

public:
	/*
		���� : collider ������ ���� �ʱ�ȭ ���ֱ�.
		Material ������Ʈ Ư���� ���� ������ �� �ֵ���. 
		�⺻�� {0.5f, 0.5f, 0.0f}
	*/
	virtual void Initialize();
	virtual void SetMaterial(float staticFriction, float DynamicFriction, float restitution) {}

	void UpdateTransform();
	//void SetObjectType(ObjectType type);
public:
	BoxCollider* m_pOwner;
	Vector3D m_Scale;
	//ObjectType m_ObjectType;

	PxRigidActor* m_pRigidActor=nullptr;
	PxTransform m_Transform;
	PxBoxGeometry m_BoxGeometry;
	PxMaterial* m_pMaterial;
	PxShape* m_pShape;
};

