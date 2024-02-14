#include "pch.h"
#include "PhysicsManager.h"
#include "WorldManager.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"

// 석영 : filter 설정 어떻게 해줄지
PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// 플레이어와 다른 물체들은 충돌 처리 / 나머지는 물리X 
	if (filterData0.word0 == Collision_Mask::PLAYER || filterData1.word0 == Collision_Mask::PLAYER)
	{
		if (filterData0.word0 == Collision_Mask::TRIGGER || filterData1.word0 == Collision_Mask::TRIGGER )
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT  | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;
		return PxFilterFlag::eDEFAULT;
	}

	return PxFilterFlag::eSUPPRESS;
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
	sceneDesc.gravity = PxVec3(0.0f, -98.1f, 0.0f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = CustomFilterShader;

	// Filter Callback 함수 등록
	FilterCallback* callback = new FilterCallback;
	sceneDesc.simulationEventCallback = callback;

	m_pPxScene = m_pPhysics->createScene(sceneDesc);

#ifdef _DEBUG
	DebugSetUp();
#endif // _DEBUG

	InitFilterData();

}

void PhysicsManager::Update(float deltatime)
{
	// 석영 : Collider 위치를 오브젝트 위치로 변경 먼저 해주기
	/*for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdateTransform();*/

	for (auto& collider : m_pStaticColliders)
		if (collider.second->m_pOwner->m_IsTrigger == false)
			collider.second->UpdateTransform();

	// 석영 : 물리 시뮬레이션 돌리기
	m_pPxScene->simulate(deltatime);
	m_pPxScene->fetchResults(true);

	// 석영 : 결과로 나온 값을 오브젝트로 넣어주기
	for (auto& collider : m_pDynamicColliders)
		if (collider.second->m_pOwner->m_IsTrigger == false)
		{
			collider.second->UpdatePhysics();
		}


	// 석영 : 이것들을 다 넘겨주고 클리어해야함

	while (!m_CollisionQue.empty())
	{
		StaticCollider* obj = m_CollisionQue.front().second;
		UserData* data = static_cast<UserData*>(obj->m_Rigid->userData);
		m_CollisionQue.pop();
	}
}

void PhysicsManager::RayCast(PxVec3 raycastPoint, PxVec3 raycastDir)
{
	PxRaycastBuffer hit;

	PxVec3 vector = { 0,0,0 };
	raycastDir.normalize();
	//PxVec3 Dir = { dir.GetX(),dir.GetY(),dir.GetZ() };
	if (raycastDir == vector)
		return;
	bool bHit = m_pPxScene->raycast(raycastPoint, raycastDir, 100.f, hit, PxHitFlag::eDEFAULT);

	if (bHit)
	{
		UserData* data = static_cast<UserData*>(hit.block.actor->userData);
		int id = data->m_EntityId;
		cout << "Entity Id : " << id << endl;
	}
}

void PhysicsManager::CreateCollider(BoxCollider* boxcollider, int entId)
{
	if (boxcollider->m_ColliderType == ColliderType::DYNAMIC)
	{
		DynamicCollider* newDynamicCollider = new DynamicCollider(boxcollider);

		newDynamicCollider->Initialize();
		m_pDynamicColliders.push_back(make_pair(entId, newDynamicCollider));

		UserData* user = new UserData;
		user->m_CollisionType = boxcollider->m_CollisionType;
		user->m_EntityId = entId;
		user->m_State = CollisionState::NONE;

		newDynamicCollider->m_Rigid->userData = user;

	}
	else if (boxcollider->m_ColliderType == ColliderType::STATIC)
	{
		StaticCollider* newStaticCollider = new StaticCollider(boxcollider);

		newStaticCollider->Initialize();
		m_pStaticColliders.push_back(make_pair(entId, newStaticCollider));

		UserData* user = new UserData;
		user->m_CollisionType = boxcollider->m_CollisionType;
		user->m_EntityId = entId;
		user->m_State = CollisionState::NONE;

		newStaticCollider->m_Rigid->userData = user;
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

void PhysicsManager::InitFilterData()
{
	PxFilterData* playerFilterData = new PxFilterData;
	playerFilterData->word0 = Collision_Mask::PLAYER;

	PxFilterData* groundFilterData = new PxFilterData;
	groundFilterData->word0 = Collision_Mask::GROUND;

	PxFilterData* wallFilterData = new PxFilterData;
	wallFilterData->word0 = Collision_Mask::WALL;

	PxFilterData* objectFilterData = new PxFilterData;
	objectFilterData->word0 = Collision_Mask::OBJECT;

	PxFilterData* blockFilterData = new PxFilterData;
	blockFilterData->word0 = Collision_Mask::TRIGGER;

	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::TRIGGER, blockFilterData));
	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::PLAYER, playerFilterData));
	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::GROUND, groundFilterData));
	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::WALL, wallFilterData));
	m_pFilterDatas.insert(std::pair<Collision_Mask, PxFilterData*>(Collision_Mask::OBJECT, objectFilterData));
}

void PhysicsManager::AddToCollisionQueue(int entId)
{
	for (const auto& obj : m_pStaticColliders) {
		int Id = obj.first;
		StaticCollider* colliderPtr = obj.second;

		if (obj.first == entId)
		{
			m_CollisionQue.push(make_pair(entId, obj.second));
		}
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

void FilterCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; ++i)
	{
		const physx::PxContactPair& pair = pairs[i];

		// Collsion Enter
		if (pair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{

			auto& pair = pairHeader.actors;

			bool player = false;
			for (auto& actor : pair)
			{
				UserData* userData = static_cast<UserData*>(actor->userData);
				if (userData->m_CollisionType == Collision_Mask::PLAYER)
					player = true;
			}

			for (auto& actor : pair)
			{
				UserData* userData = static_cast<UserData*>(actor->userData);
				userData->m_State = CollisionState::ENTER;

				if (player && userData->m_CollisionType != Collision_Mask::PLAYER)
				{
					PhysicsManager::GetInstance()->AddToCollisionQueue(userData->m_EntityId);
					player = false;
				}
			}
		}

		// Collsion Stay
		if (pair.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			auto& pair = pairHeader.actors;

			bool player = false;
			for (auto& actor : pair)
			{
				UserData* userData = static_cast<UserData*>(actor->userData);
				if (userData->m_CollisionType == Collision_Mask::PLAYER)
					player = true;
			}

			for (auto& actor : pair)
			{
				UserData* userData = static_cast<UserData*>(actor->userData);
				userData->m_State = CollisionState::STAY;

				if (player && userData->m_CollisionType != Collision_Mask::PLAYER)
				{
					PhysicsManager::GetInstance()->AddToCollisionQueue(userData->m_EntityId);
					player = false;
				}
			}
		}

		// Collsion Exit
		if (pair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			auto& pair = pairHeader.actors;

			for (auto& actor : pair)
			{
				UserData* userData = static_cast<UserData*>(actor->userData);
				userData->m_State = CollisionState::EXIT;
			}
		}

	}
}

void FilterCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	for (PxU32 i = 0; i < count; i++)
	{
		const PxTriggerPair& pair = pairs[i];

		if (pair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			UserData* userData = static_cast<UserData*>(pair.triggerActor->userData);
			userData->m_State = CollisionState::ENTER;
			PhysicsManager::GetInstance()->AddToCollisionQueue(userData->m_EntityId);
		}
		// not...supported....why...why.....................................
		/*if (pair.status & PxPairFlag::eDETECT_DISCRETE_CONTACT)
		{
			UserData* userData = static_cast<UserData*>(pair.triggerActor->userData);
			userData->m_State = CollisionState::STAY;
			PhysicsManager::GetInstance()->AddToCollisionQueue(userData->m_EntityId);
		}*/
		if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			UserData* userData = static_cast<UserData*>(pair.triggerActor->userData);
			userData->m_State = CollisionState::EXIT;
		}
	}
}

