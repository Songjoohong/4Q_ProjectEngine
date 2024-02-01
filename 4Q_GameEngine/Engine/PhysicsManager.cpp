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

	// 석영 : 일단 씬이 하나일 예정이니까 한개만 PxScene 생성 
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -981.f, 0.0f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	// 석영 : 후에 씬이 여러개로 작업된다면 바꿀 예정
	m_pCurrentPxScene = m_pPhysics->createScene(sceneDesc); 
}

void PhysicsManager::Update(float deltatime)
{
	// 석영 : Collider 위치를 오브젝트 위치로 변경 먼저 해주기
	for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdateTransform();

	for (auto& collider : m_pStaticColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdateTransform();

	// 석영 : 물리 시뮬레이션 돌리기
	m_pCurrentPxScene->simulate(deltatime);
	m_pCurrentPxScene->fetchResults(true);

	// 석영 : 결과로 나온 값을 오브젝트로 넣어주기
	for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdatePhysics();
}

void PhysicsManager::ChangePxScene()
{
	// 석영 : 바뀐 월드에 PxScene을 가지고 온다.
	ECS::World* world = WorldManager::GetInstance()->GetCurrentWorld();
	m_pCurrentPxScene = m_pPxScenes[world];

	
	/*
		석영 : 추가적으로 작업해야함. -> Scene이 여러개로 된다는 가정하에 작업해야함.
		 ex) 플레이어 또는 사물이 Scene을 이동하는 경우
		 이전 PxScene 에서 removeActor를 해준 후
		 현재 PxScene 에 AddActor를 해줘야한다.
	*/
}

void PhysicsManager::CreateCollider(BoxCollider* boxcollider, PhysicsType type)
{
	if (type == PhysicsType::DYNAMIC)
	{
		DynamicCollider* newDynamicCollider = new DynamicCollider(boxcollider);

		// 석영 : Scene 하나라는 가정하에 작업.
		m_pDynamicColliders.push_back(newDynamicCollider);
		m_pCurrentPxScene->addActor(*(newDynamicCollider->m_Rigid));
	}
	else if (type == PhysicsType::STATIC)
	{
		StaticCollider* newStaticCollider = new StaticCollider(boxcollider);

		// 석영 : Scene 하나라는 가정하에 작업.
		m_pStaticColliders.push_back(newStaticCollider);
		m_pCurrentPxScene->addActor(*(newStaticCollider->m_Rigid));
	}
}

void PhysicsManager::DebugSetUp()
{
	// PhysX Visual Debbugger 보기위한 세팅
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
