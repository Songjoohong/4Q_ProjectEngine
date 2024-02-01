#pragma once
#include "ECS.h"
#include "ISingleton.h"

#include <map>
#include <vector>

/*
	//석영//
	Todo : 까먹지 말고 해둬야하는 것들
	1. PX_RELEASE 해주기.
	2. 물리 컴포넌트 씬 바뀔때마다 삭제 / 추가 기능 하기.
*/

enum PhysicsType
{
	DYNAMIC,
	STATIC
};

using namespace physx;
using namespace std;

class StaticCollider;
class DynamicCollider;
class PhysicsManager : public ISingleton<PhysicsManager>
{
public: 
	PhysicsManager() = default;
	~PhysicsManager() override = default;

public:
	void Initialize();
	void Update(float deltatime);

	void ChangePxScene();
	void CreateCollider(BoxCollider* boxcollider,PhysicsType type);
	void DebugSetUp();

public:
	PxPvd* m_pPvd = nullptr; // PhysX Visual Debbugger -> 시각화 하여 디버깅 할 수 있는 프로그램
	PxFoundation* m_pFoundation = nullptr;
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;

	PxPhysics* m_pPhysics = nullptr;

	// PxScene 관련
	std::map<ECS::World*,PxScene*> m_pPxScenes; 
	PxScene* m_pCurrentPxScene=nullptr;

	// Colliders
	vector<DynamicCollider*> m_pDynamicColliders;
	vector<StaticCollider*> m_pStaticColliders;
};

