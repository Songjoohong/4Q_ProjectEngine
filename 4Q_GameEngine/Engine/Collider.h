#pragma once
#include "BoxCollider.h"
using namespace physx; 
using namespace Component; // ���� : main ������ ������ �ȴ�.

/*
	���� : AddActor ���ֱ�! ��� ������..?
	PxTranform ��ġ�� ���� �Ʒ��������� �ݶ��̴��� �����ȴ�.
*/

class Collider
{
public:

	Collider(BoxCollider* owner);
	virtual ~Collider() = default;

public:
	/*
		���� : collider ������ ���� �ʱ�ȭ ���ֱ�.
		Material ������Ʈ Ư���� ���� ������ �� �ֵ���. 
		�⺻�� {1.0f, 1.0f, 0.0f}
	*/
	virtual void Initialize();
	virtual void SetMaterial(float staticFriction, float DynamicFriction, float restitution) {}

public:
	BoxCollider* m_pOwner;
	Vector3D m_Scale;

	PxTransform m_Transform;
	PxBoxGeometry m_BoxGeometry;
	PxMaterial* m_pMaterial;
};

