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
	virtual ~DynamicCollider();

public:
	virtual void Initialize() override;

	void UpdatePhysics();

	/*
	* ����: ��ü ���� ����->�⺻ 60.f �� ����.
	* ���� �־��ָ� density ����ؼ� ������Ʈ ���ش�.
	*/ 
	void SetDensity(float mass);

	// ���� : �̵��� ���� �Լ�
	void AddForce(Vector3D dir);

	// ���� : ���� ȿ������ ȸ���� ������
	void FreezeRotationX(bool active);
	void FreezeRotationY(bool active);
	void FreezeRotationZ(bool active);
public:
	PxRigidDynamic* m_Rigid;
};

