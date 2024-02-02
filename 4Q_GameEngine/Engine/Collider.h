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
		�⺻�� {0.5f, 0.5f, 0.0f}
	*/
	virtual void Initialize();
	virtual void SetMaterial(float staticFriction, float DynamicFriction, float restitution) {}

	void UpdateTransform();
public:
	BoxCollider* m_pOwner;
	Vector3D m_Scale;

	PxTransform m_Transform;
	PxBoxGeometry m_BoxGeometry;
	PxMaterial* m_pMaterial;
};

