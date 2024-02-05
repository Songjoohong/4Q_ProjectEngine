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

	// Contact modifier ���
	m_pContactModifier = new ContactModifier;
	m_pPxScene->setContactModifyCallback(m_pContactModifier);

	// FilterData 
	PxFilterData* filterDataPlayer;
	filterDataPlayer->word0 = ObjectType::PLAYER;
	filterDataPlayer->word1 = ObjectType::SLOPE;

	m_pFilterDatas[ObjectType::PLAYER] = filterDataPlayer;

	PxFilterData* filterDataSlope;
	filterDataSlope->word0 = ObjectType::SLOPE;
	filterDataSlope->word1 = ObjectType::PLAYER;

	m_pFilterDatas[ObjectType::SLOPE] = filterDataSlope;

	PxFilterData* filterData;
	filterDataSlope->word0 = ObjectType::GROUND | ObjectType::WALL | ObjectType::OBJECT;
	filterDataSlope->word1 = ObjectType::PLAYER;

	m_pFilterDatas[ObjectType::OBJECT] = filterData;
	m_pFilterDatas[ObjectType::GROUND] = filterData;
	m_pFilterDatas[ObjectType::WALL] = filterData;

	// UserData
	UserData* player=new UserData;
	player->m_Type = ObjectType::PLAYER;
	m_pUserDatas[ObjectType::PLAYER] = player;

	UserData* Ground = new UserData;
	Ground->m_Type = ObjectType::GROUND;
	m_pUserDatas[ObjectType::GROUND] = Ground;

	UserData* Slope = new UserData;
	Slope->m_Type = ObjectType::SLOPE;
	m_pUserDatas[ObjectType::SLOPE] = Slope;

	UserData* Wall = new UserData;
	Wall->m_Type = ObjectType::WALL;
	m_pUserDatas[ObjectType::WALL] = Wall;
	
	UserData* Object = new UserData;
	Object->m_Type = ObjectType::OBJECT;
	m_pUserDatas[ObjectType::OBJECT] = Object;
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