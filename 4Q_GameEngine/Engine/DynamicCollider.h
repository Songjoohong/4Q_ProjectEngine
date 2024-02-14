#pragma once
#include "Collider.h"

/*
	석영 : 플레이어의 경우 키 입력을 받아야하는데..?
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
	* 석영: 물체 질량 설정->기본 60.f 로 설정.
	* 질량 넣어주면 density 계산해서 업데이트 해준다.
	*/ 
	void SetDensity(float mass);

	// 석영 : 이동을 위한 함수
	void AddForce(Vector3D dir);

	// 석영 : 물리 효과에서 회전축 고정용
	void FreezeRotation(bool x_active,bool y_active,bool z_active);
	void FreezeLinear(bool x_active,bool y_active,bool z_active);
public:
	PxRigidDynamic* m_Rigid;

	bool m_bKeyUp = false;
	float m_MaxVelocity = 150.f;
	PxVec3 m_CurrentDir = { 0.f,0.f,0.f };
	PxVec3 m_PrevDir = { 0.f,0.f,0.f };
};

