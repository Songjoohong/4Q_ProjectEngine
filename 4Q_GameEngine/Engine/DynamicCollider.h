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
	~DynamicCollider();

public:
	/*
		석영 :
		충돌처리
		위치 업데이트를 시켜줘야 한다.
		오브젝트에게 위치값을 전달해야한다.
	*/

	virtual void Initialize();

	void UpdatePhysics();
	void UpdateTransform();

	// 석영 : 물체 질량 설정 -> 기본 0.3f 로 설정.
	void SetMass(float mass);

	// 석영 : 이동을 위한 함수
	void AddForce(Vector3D dir);

	// 석영 : 물리 효과에서 회전축 고정용
	void FreezeRotationX(bool active);
	void FreezeRotationY(bool active);
	void FreezeRotationZ(bool active);
public:
	PxRigidDynamic* m_Rigid;
};

