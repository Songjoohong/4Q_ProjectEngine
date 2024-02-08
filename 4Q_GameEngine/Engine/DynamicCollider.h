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
	virtual ~DynamicCollider() override = default;

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

	bool m_bKeyUp = false;
	PxVec3 m_CurrentDir = { 0.f,0.f,0.f };
	PxVec3 m_PrevDir = { 0.f,0.f,0.f };
};

