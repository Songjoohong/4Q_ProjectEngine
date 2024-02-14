#pragma once
#include "ECS.h"
#include "ISingleton.h"

#include <map>
#include <vector>

#include "BoxCollider.h"

using namespace physx;
using namespace std;

class StaticCollider;
class DynamicCollider;
class PhysicsManager : public ISingleton<PhysicsManager>
{
public:
	PhysicsManager() = default;
	~PhysicsManager() override;

public:
	void Initialize();
	void Update(float deltatime);

	void CreateCollider(BoxCollider* boxcollider, int entId);
	void DebugSetUp();
	void InitFilterData();

	PxPhysics* GetPhysics() { return m_pPhysics; }
	PxScene* GetPxScene() { return m_pPxScene; }
	PxFilterData* GetFilterData(Collision_Mask type) { return m_pFilterDatas[type]; }
	DynamicCollider* GetDynamicCollider(int entId);
	//CollisionState GetCollisionState(int entId);

	PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);
public:
	PxPvd* m_pPvd = nullptr; // PhysX Visual Debbugger -> 시각화 하여 디버깅 할 수 있는 프로그램
	PxFoundation* m_pFoundation = nullptr;
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;

private:
	PxPhysics* m_pPhysics = nullptr;
	PxScene* m_pPxScene = nullptr;

	// Colliders
	vector<pair<int, DynamicCollider*>> m_pDynamicColliders;
	vector<pair<int, StaticCollider*>> m_pStaticColliders;

	// Filter
	map<Collision_Mask, PxFilterData*> m_pFilterDatas;
};

class FilterCallback
	:public PxSimulationEventCallback
{
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

	//  안쓸예정
	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override {}
	virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override{}
	virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override{}
	virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override{}
	virtual void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) override{}
};