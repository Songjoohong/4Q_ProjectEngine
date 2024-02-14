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
	virtual void SetFilterData() override;

	void UpdatePhysics();
	void SetFilterData();

	/*
	* ����: ��ü ���� ����->�⺻ 60.f �� ����.
	* ���� �־��ָ� density ����ؼ� ������Ʈ ���ش�.
	*/ 
	void SetDensity(float mass);

	// ���� : �̵��� ���� �Լ�
	void AddForce(Vector3D dir);

	// ���� : ���� ȿ������ ȸ���� ������
	void FreezeRotation(bool x_active,bool y_active,bool z_active);
	void FreezeLinear(bool x_active,bool y_active,bool z_active);
public:
	PxRigidDynamic* m_Rigid;

	bool m_bKeyUp = false;
	float m_MaxVelocity = 150.f;
	PxVec3 m_CurrentDir = { 0.f,0.f,0.f };
	PxVec3 m_PrevDir = { 0.f,0.f,0.f };
};

