#pragma once
#include "ECS.h"
#include "ISingleton.h"

#include <map>
#include <vector>
#include <queue>

#include "BoxCollider.h"

using namespace physx;
using namespace std;

struct  UserData;
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
	void RayCast(PxVec3 raycastPoint, PxVec3 raycastDir);

	void ChangeCollider(BoxCollider* boxcollider, int entId);
	void ChangeFilter(int entId);
	void ChangeColliderPosition(BoxCollider* boxcolldier, int entId);
	void CreateCollider(BoxCollider* boxcollider, int entId);
	void DebugSetUp();
	void InitFilterData();
	void AddCollisionColliders(int entId);
	void DeleteCollisionCollider(int entId);

	vector<pair<int, StaticCollider*>> GetCollider();
	PxPhysics* GetPhysics() { return m_pPhysics; }
	PxScene* GetPxScene() { return m_pPxScene; }
	PxFilterData* GetFilterData(CollisionType type) { return m_pFilterDatas[type]; }
	DynamicCollider* GetDynamicCollider(int entId);
	DynamicCollider* GetPlayerCollider() { return m_PlayerCollider; }


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
	vector<pair<int, StaticCollider*>> m_CollisionObjects;

	// Player 일단 따로 저장 -> 시간되면 빼놓기
	DynamicCollider* m_PlayerCollider;

	map<CollisionType, PxFilterData*> m_pFilterDatas;
};

class FilterCallback
	:public PxSimulationEventCallback
{
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;

	//  안쓸예정
	virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {}
	virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {}
	virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override {}
	virtual void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) override {}
};