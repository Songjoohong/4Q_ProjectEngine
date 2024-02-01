#include "pch.h"
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

	// ���� : �ϴ� ���� �ϳ��� �����̴ϱ� �Ѱ��� PxScene ���� 
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -981.f, 0.0f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	// ���� : �Ŀ� ���� �������� �۾��ȴٸ� �ٲ� ����
	m_pCurrentPxScene = m_pPhysics->createScene(sceneDesc); 
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
	m_pCurrentPxScene->simulate(deltatime);
	m_pCurrentPxScene->fetchResults(true);

	// ���� : ����� ���� ���� ������Ʈ�� �־��ֱ�
	for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdatePhysics();
}

void PhysicsManager::ChangePxScene()
{
	// ���� : �ٲ� ���忡 PxScene�� ������ �´�.
	ECS::World* world = WorldManager::GetInstance()->GetCurrentWorld();
	m_pCurrentPxScene = m_pPxScenes[world];

	
	/*
		���� : �߰������� �۾��ؾ���. -> Scene�� �������� �ȴٴ� �����Ͽ� �۾��ؾ���.
		 ex) �÷��̾� �Ǵ� �繰�� Scene�� �̵��ϴ� ���
		 ���� PxScene ���� removeActor�� ���� ��
		 ���� PxScene �� AddActor�� ������Ѵ�.
	*/
}

void PhysicsManager::CreateCollider(BoxCollider* boxcollider, PhysicsType type)
{
	if (type == PhysicsType::DYNAMIC)
	{
		DynamicCollider* newDynamicCollider = new DynamicCollider(boxcollider);

		// ���� : Scene �ϳ���� �����Ͽ� �۾�.
		m_pDynamicColliders.push_back(newDynamicCollider);
		m_pCurrentPxScene->addActor(*(newDynamicCollider->m_Rigid));
	}
	else if (type == PhysicsType::STATIC)
	{
		StaticCollider* newStaticCollider = new StaticCollider(boxcollider);

		// ���� : Scene �ϳ���� �����Ͽ� �۾�.
		m_pStaticColliders.push_back(newStaticCollider);
		m_pCurrentPxScene->addActor(*(newStaticCollider->m_Rigid));
	}
}

void PhysicsManager::DebugSetUp()
{
	// PhysX Visual Debbugger �������� ����
	m_pPvd = PxCreatePvd(*m_pFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPvd);

	PxPvdSceneClient* pvdClient = m_pCurrentPxScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}
