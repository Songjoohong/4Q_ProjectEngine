#pragma once
#include "ECS.h"
#include "ISingleton.h"

#include <map>
#include <vector>

/*
	//����//
	Todo : ����� ���� �ص־��ϴ� �͵�
	1. PX_RELEASE ���ֱ�.
	2. ���� ������Ʈ �� �ٲ𶧸��� ���� / �߰� ��� �ϱ�.
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
	PxPvd* m_pPvd = nullptr; // PhysX Visual Debbugger -> �ð�ȭ �Ͽ� ����� �� �� �ִ� ���α׷�
	PxFoundation* m_pFoundation = nullptr;
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;

	PxPhysics* m_pPhysics = nullptr;

	// PxScene ����
	std::map<ECS::World*,PxScene*> m_pPxScenes; 
	PxScene* m_pCurrentPxScene=nullptr;

	// Colliders
	vector<DynamicCollider*> m_pDynamicColliders;
	vector<StaticCollider*> m_pStaticColliders;
};

