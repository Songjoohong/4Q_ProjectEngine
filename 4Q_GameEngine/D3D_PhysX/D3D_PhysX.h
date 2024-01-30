#pragma once
#include <vector>

using namespace physx;
using namespace std;

/*
	//석영//
	Todo : 까먹지 말고 해둬야하는 것들
	1. PX_RELEASE 해주기.
	2. 물리 컴포넌트 씬 바뀔때마다 삭제 / 추가 기능 하기.
*/

class Model;
class StaticCollider;
class DynamicCollider;
class Physics
{
public:
	~Physics()=default;

public:
	void Initialize();
	void Update(float deltatime);

	Physics* GetInstance();
	void StepPhysics(float deltatime);

private:
	vector<StaticCollider*> m_StaticColliders;
	vector<DynamicCollider*> m_DynamicColliders;

private:
	//PxPvd* m_pPvd = nullptr; // PhysX Visual Debbugger -> 시각화 하여 디버깅 할 수 있는 프로그램?
	PxFoundation*			m_pFoundation = nullptr;
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;

	PxPhysics*			    m_pPhysics = nullptr;
	PxScene*				m_pScene = nullptr;
	PxMaterial*				m_pMaterial = nullptr;

	static Physics*			m_pInstance;

	Physics() = default;
};