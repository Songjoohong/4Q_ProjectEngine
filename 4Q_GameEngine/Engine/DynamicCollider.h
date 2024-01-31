#pragma once
#include "Collider.h"

/*
	���� : �÷��̾��� ��� Ű �Է��� �޾ƾ��ϴµ�..?
*/

class DynamicCollider
	:public Collider
{
public:
	DynamicCollider(BoxCollider* owner);
	~DynamicCollider();

public:
	/*
		���� :
		�浹ó��
		��ġ ������Ʈ�� ������� �Ѵ�.
		������Ʈ���� ��ġ���� �����ؾ��Ѵ�.
	*/

	virtual void Initialize();

	void UpdatePhysics();
	void UpdateTransform();

	// ���� : ��ü ���� ���� -> �⺻ 0.3f �� ����.
	void SetMass(float mass);

	// ���� : �̵��� ���� �Լ�
	void AddForce(Vector3D dir);

	// ���� : ���� ȿ������ ȸ���� ������
	void FreezeRotationX(bool active);
	void FreezeRotationY(bool active);
	void FreezeRotationZ(bool active);
public:
	PxRigidDynamic* m_Rigid;
};

