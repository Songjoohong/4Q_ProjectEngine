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
	void RayCast(Vector3D dir);

	void InitFilterDatas();
	void CreateCollider(BoxCollider* boxcollider, int entId);
	void DebugSetUp();
	
	PxPhysics* GetPhysics() { return m_pPhysics; }
	PxScene* GetPxScene() { return m_pPxScene; }
	PxFilterData* GetFilterData(Collision_Mask type) { return m_pFilterDatas[type]; }
	DynamicCollider* GetDynamicCollider(int entId);

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
	vector<StaticCollider*> m_pStaticColliders;

	// Filter 관련 ---석영 : 일단 이렇게 해놓고..깔끔하게 바꿀 수 있으면 바꿀게요..
	map<Collision_Mask, PxFilterData*> m_pFilterDatas;
	PxU32 m_Collision_Mask_Player;
	PxU32 m_Collision_Mask_Ground;
	PxU32 m_Collision_Mask_Slope;
	PxU32 m_Collision_Mask_Object;
	PxU32 m_Collision_Mask_Block;
};

