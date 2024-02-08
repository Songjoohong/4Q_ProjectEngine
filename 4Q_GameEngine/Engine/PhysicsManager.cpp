#include "pch.h"
#include "PhysicsManager.h"
#include "WorldManager.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"

void PhysicsManager::Initialize()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
#ifdef _DEBUG
	// PhysX Visual Debbugger �������� ����
	m_pPvd = PxCreatePvd(*m_pFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPvd);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, nullptr);
#endif // _DEBUG
	// ���� : PxScene ���� 
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -981.f, 0.0f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	m_pPxScene = m_pPhysics->createScene(sceneDesc);

#ifdef _DEBUG
	DebugSetUp();
#endif // _DEBUG
}

void PhysicsManager::Update(float deltatime)
{
	// ���� : Collider ��ġ�� ������Ʈ ��ġ�� ���� ���� ���ֱ�
	/*for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdateTransform();*/

	for (auto& collider : m_pStaticColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdateTransform();

	// ���� : ���� �ùķ��̼� ������
	m_pPxScene->simulate(deltatime);
	m_pPxScene->fetchResults(true);

	// ���� : ����� ���� ���� ������Ʈ�� �־��ֱ�
	for (auto& collider : m_pDynamicColliders)
		if (collider.second->m_pOwner->m_IsTrigger == false)
		{
			collider.second->UpdatePhysics();
		}
}

void PhysicsManager::CreateCollider(BoxCollider* boxcollider, int entId)
{
	if (boxcollider->m_CollisionType == CollisionType::DYNAMIC)
	{
		DynamicCollider* newDynamicCollider = new DynamicCollider(boxcollider);

		newDynamicCollider->Initialize();
		m_pDynamicColliders.push_back(make_pair(entId, newDynamicCollider));
	}
	else if (boxcollider->m_CollisionType == CollisionType::STATIC)
	{
		StaticCollider* newStaticCollider = new StaticCollider(boxcollider);

		newStaticCollider->Initialize();
		m_pStaticColliders.push_back(newStaticCollider);
	}
}

void PhysicsManager::DebugSetUp()
{
	// PhysX Visual Debbugger �������� ����

	PxPvdSceneClient* pvdClient = m_pPxScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

DynamicCollider* PhysicsManager::GetDynamicCollider(int entId)
{
	const auto it = std::find_if(m_pDynamicColliders.begin(), m_pDynamicColliders.end(), [entId](pair<int, DynamicCollider*> collider)
		{
			return collider.first == entId;
		});
	return it->second;
}

// ���� : filter ���� ��� ������
PxFilterFlags PhysicsManager::CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	if (filterData0.word0 == Collision_Mask::IS_TRIGGER || filterData1.word0 == Collision_Mask::IS_TRIGGER)
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
	else
		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	return PxFilterFlag::eDEFAULT;
}


PhysicsManager::~PhysicsManager()
{
	PX_RELEASE(m_pPxScene);
	PX_RELEASE(m_pDispatcher);
	PxCloseExtensions();
	PX_RELEASE(m_pPhysics);
	if (m_pPvd)
	{
		PxPvdTransport* transport = m_pPvd->getTransport();
		m_pPvd->release();	m_pPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(m_pFoundation);
}
