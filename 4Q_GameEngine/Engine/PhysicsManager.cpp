#include "pch.h"
#include "PxEnum.h"
#include "PhysicsManager.h"
#include "WorldManager.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"

void PhysicsManager::Initialize()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
#ifdef _DEBUG
	DebugSetUp();
#endif // _DEBUG

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, nullptr);

	// ���� : PxScene ���� 
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -981.f, 0.0f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	m_pPxScene = m_pPhysics->createScene(sceneDesc); 
}

void PhysicsManager::Update(float deltatime)
{
	// ���� : Collider ��ġ�� ������Ʈ ��ġ�� ���� ���� ���ֱ�
	for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdateTransform();

	for (auto& collider : m_pStaticColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdateTransform();

	// ���� : ���� �ùķ��̼� ������
	m_pPxScene->simulate(deltatime);
	m_pPxScene->fetchResults(true);

	// ���� : ����� ���� ���� ������Ʈ�� �־��ֱ�
	for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdatePhysics();
}

void PhysicsManager::CreateCollider(BoxCollider* boxcollider, PhysicsType type)
{
	if (type == PhysicsType::DYNAMIC)
	{
		DynamicCollider* newDynamicCollider = new DynamicCollider(boxcollider);

		newDynamicCollider->Initialize();
		m_pDynamicColliders.push_back(newDynamicCollider);
	}
	else if (type == PhysicsType::STATIC)
	{
		StaticCollider* newStaticCollider = new StaticCollider(boxcollider);

		newStaticCollider->Initialize();
		m_pStaticColliders.push_back(newStaticCollider);
	}
}

void PhysicsManager::DebugSetUp()
{
	// PhysX Visual Debbugger �������� ����
	m_pPvd = PxCreatePvd(*m_pFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPvd);

	PxPvdSceneClient* pvdClient = m_pPxScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
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