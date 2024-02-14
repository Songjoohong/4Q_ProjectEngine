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

	// Player ���� --> �ð� �ȴٸ� ���� ����
	void CheckObject(Vector3D dir);
	PxVec3 CheckSlope(PxVec3 dir);

	void InitFilterDatas();
	void CreateCollider(BoxCollider* boxcollider, int entId);
	void DebugSetUp();

	PxPhysics* GetPhysics() { return m_pPhysics; }
	PxScene* GetPxScene() { return m_pPxScene; }
	PxFilterData* GetFilterData(Collision_Mask type) { return m_pFilterDatas[type]; }
	DynamicCollider* GetDynamicCollider(int entId);

public:
	PxPvd* m_pPvd = nullptr; // PhysX Visual Debbugger -> �ð�ȭ �Ͽ� ����� �� �� �ִ� ���α׷�
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

	// Filter ���� ---���� : �ϴ� �̷��� �س���..����ϰ� �ٲ� �� ������ �ٲܰԿ�..
	map<Collision_Mask, PxFilterData*> m_pFilterDatas;

};

