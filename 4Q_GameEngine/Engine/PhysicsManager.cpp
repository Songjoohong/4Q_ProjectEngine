#include "pch.h"
#include "PhysicsManager.h"
#include "WorldManager.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"

PxU32 g_Collision_Mask_Player=1;
PxU32 g_Collision_Mask_Ground=2;
PxU32 g_Collision_Mask_Slope=3;
PxU32 g_Collision_Mask_Object=4;
PxU32 g_Collision_Mask_Block=5;

PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// 플레이어와 다른 물체들은 충돌 처리 / 나머지는 물리X 
	if (filterData0.word0 == g_Collision_Mask_Player || filterData1.word0 == g_Collision_Mask_Player)
	{
		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	else
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eSUPPRESS;
	}
}

void PhysicsManager::Initialize()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
#ifdef _DEBUG
	// PhysX Visual Debbugger 보기위한 세팅
	m_pPvd = PxCreatePvd(*m_pFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPvd);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, nullptr);
#endif // _DEBUG
	// 석영 : PxScene 생성 
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -981.f, 0.0f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = CustomFilterShader;

	m_pPxScene = m_pPhysics->createScene(sceneDesc);

#ifdef _DEBUG
	DebugSetUp();
#endif // _DEBUG

	InitFilterDatas();
}

void PhysicsManager::Update(float deltatime)
{
	// 석영 : Collider 위치를 오브젝트 위치로 변경 먼저 해주기
	/*for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdateTransform();*/

	for (auto& collider : m_pStaticColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdateTransform();

	// 석영 : 물리 시뮬레이션 돌리기
	m_pPxScene->simulate(deltatime);
	m_pPxScene->fetchResults(true);

	// 석영 : 결과로 나온 값을 오브젝트로 넣어주기
	for (auto& collider : m_pDynamicColliders)
		if (collider.second->m_pOwner->m_IsTrigger == false)
		{
			collider.second->UpdatePhysics();
		}
}

void PhysicsManager::RayCast(Vector3D dir)
{
	PxRaycastBuffer hit;
	PxVec3 Dir = { dir.GetX(),dir.GetY(),dir.GetZ() };
	bool bHit = m_pPxScene->raycast(Dir, PxVec3(0.f, 0.f, 1.f), 5.f, hit, PxHitFlag::eDEFAULT);

	if (bHit)
	{
		void* data = hit.block.actor->userData;
		void* checkData = (void*)g_Collision_Mask_Object;

		assert(data != checkData);
	}
}

void PhysicsManager::InitFilterDatas()
{
	PxFilterData* playerFilterData = new PxFilterData;
	playerFilterData->word0 = g_Collision_Mask_Player;

	PxFilterData* groundFilterData = new PxFilterData;
	groundFilterData->word0 = g_Collision_Mask_Ground;

	PxFilterData* slopeFilterData = new PxFilterData;
	slopeFilterData->word0 = g_Collision_Mask_Slope;

	PxFilterData* objectFilterData = new PxFilterData;
	objectFilterData->word0 = g_Collision_Mask_Object;

	PxFilterData* blockFilterData = new PxFilterData;
	blockFilterData->word0 = g_Collision_Mask_Block;

	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::BLOCK, blockFilterData));
	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::PLAYER, playerFilterData));
	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::GROUND, groundFilterData));
	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::SLOPE, slopeFilterData));
	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::OBJECT, objectFilterData));
	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::WALL, blockFilterData));
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
	// PhysX Visual Debbugger 보기위한 세팅

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
