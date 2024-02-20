#include "pch.h"
#include "PhysicsManager.h"
#include "WorldManager.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"

// 석영 : filter 설정 어떻게 해줄지
PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	printf("Filtering: %u, %u\n", filterData0.word0, filterData1.word0);

	// 플레이어와 다른 물체들은 충돌 처리 / 나머지는 물리X 
	if (filterData0.word0 == CollisionType::PLAYER || filterData1.word0 == CollisionType::PLAYER)
	{
		if (filterData0.word0 == CollisionType::TRIGGER || filterData1.word0 == CollisionType::TRIGGER|| filterData0.word0 == CollisionType::ROOM || filterData1.word0 == CollisionType::ROOM)
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;
		return PxFilterFlag::eDEFAULT;
	}

	return PxFilterFlag::eDEFAULT;
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

#endif // _DEBUG
	// 석영 : PxScene 생성
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, nullptr);
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
	if (!m_pStaticColliders.empty())
	{
		for (auto& collider : m_pStaticColliders)
		{
			PxVec3 tras = collider.second->m_Transform.p;
			collider.second->UpdateScale();
			collider.second->UpdateRotation();
			collider.second->UpdatePosition();
		}
	}

	if (!m_pDynamicColliders.empty())
	{
		for (auto& collider : m_pDynamicColliders)
		{
			collider.second->UpdateScale();
			collider.second->UpdateRotation();
			collider.second->UpdatePosition();
		}
	}
	// 석영 : 물리 시뮬레이션 돌리기
	m_pPxScene->simulate(deltatime);
	m_pPxScene->fetchResults(true);

	// 석영 : 결과로 나온 값을 오브젝트로 넣어주기
	if (!m_pDynamicColliders.empty())
	{
		for (auto& collider : m_pDynamicColliders)
			collider.second->UpdatePhysics();
	}

}

void PhysicsManager::RayCast(PxVec3 raycastPoint, PxVec3 raycastDir)
{
	PxRaycastBuffer hit;
	PxVec3 vector = { 0,0,0 };
	raycastDir.normalize();
	if (raycastDir == vector)
		return;
	bool bHit = m_pPxScene->raycast(raycastPoint, raycastDir, 150.f, hit);

	if (bHit)
	{
		UserData* data = static_cast<UserData*>(hit.block.actor->userData);
		int id = data->m_EntityId;
		cout << "Entity Id : " << id << endl;

		for (const auto& obj : m_pStaticColliders) {
			int Id = obj.first;
			StaticCollider* colliderPtr = obj.second;

			colliderPtr->m_pOwner->m_WasRaycastHit = colliderPtr->m_pOwner->m_IsRaycastHit;
			colliderPtr->m_pOwner->m_IsRaycastHit = false;

			if (obj.first == id)
			{
				colliderPtr->m_pOwner->m_IsRaycastHit = true;
			}
		}
	}
	else
	{
		// bHit = flase 일 때 모든 객체 false를 해준다.
		for (const auto& obj : m_pStaticColliders) {
			StaticCollider* colliderPtr = obj.second;

			colliderPtr->m_pOwner->m_WasRaycastHit = colliderPtr->m_pOwner->m_IsRaycastHit;
			colliderPtr->m_pOwner->m_IsRaycastHit = false;
		}
	}

}

void PhysicsManager::ChangeCollider(BoxCollider* boxcollider, int entId)
{
	for (auto it = m_pStaticColliders.begin(); it != m_pStaticColliders.end(); ++it) {
		int Id = it->first;
		StaticCollider* colliderPtr = it->second;
		if (Id == entId) {
			delete colliderPtr;
			m_pStaticColliders.erase(it);
			CreateCollider(boxcollider, entId);
			return;
		}
	}

	for (auto it = m_pDynamicColliders.begin(); it != m_pDynamicColliders.end(); ++it) {
		int Id = it->first;
		DynamicCollider* colliderPtr = it->second;

		if (Id == entId) {
			delete colliderPtr;
			m_pDynamicColliders.erase(it);
			CreateCollider(boxcollider, entId);
			return;
		}
	}
}

void PhysicsManager::ChangeFilter(int entId)
{
	for (const auto& obj : m_pStaticColliders) {
		int Id = obj.first;
		StaticCollider* colliderPtr = obj.second;

		if (obj.first == entId)
		{
			colliderPtr->SetFilterData();
			return;
		}
	}

	for (const auto& obj : m_pDynamicColliders) {
		int Id = obj.first;
		DynamicCollider* colliderPtr = obj.second;

		if (obj.first == entId)
		{
			colliderPtr->SetFilterData();
			return;
		}
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

		if (boxcollider->m_CollisionType == CollisionType::PLAYER)
			m_PlayerCollider = newDynamicCollider;

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
	playerFilterData->word0 = CollisionType::PLAYER;

	PxFilterData* groundFilterData = new PxFilterData;
	groundFilterData->word0 = CollisionType::GROUND;

	PxFilterData* wallFilterData = new PxFilterData;
	wallFilterData->word0 = CollisionType::WALL;

	PxFilterData* objectFilterData = new PxFilterData;
	objectFilterData->word0 = CollisionType::OBJECT;

	PxFilterData* blockFilterData = new PxFilterData;
	blockFilterData->word0 = CollisionType::TRIGGER;

	PxFilterData* roomFilterData = new PxFilterData;
	roomFilterData->word0 = CollisionType::ROOM;

	m_pFilterDatas.insert(std::pair<CollisionType, PxFilterData*>(CollisionType::TRIGGER, blockFilterData));
	m_pFilterDatas.insert(std::pair<CollisionType, PxFilterData*>(CollisionType::PLAYER, playerFilterData));
	m_pFilterDatas.insert(std::pair<CollisionType, PxFilterData*>(CollisionType::GROUND, groundFilterData));
	m_pFilterDatas.insert(std::pair<CollisionType, PxFilterData*>(CollisionType::WALL, wallFilterData));
	m_pFilterDatas.insert(std::pair<CollisionType, PxFilterData*>(CollisionType::OBJECT, objectFilterData));
	m_pFilterDatas.insert(std::pair<CollisionType, PxFilterData*>(CollisionType::ROOM, roomFilterData));
}

void PhysicsManager::AddCollisionColliders(int entId)
{
	for (const auto& obj : m_pStaticColliders) {
		int Id = obj.first;
		StaticCollider* colliderPtr = obj.second;

		if (obj.first == entId)
		{
			UserData* data = static_cast<UserData*>(colliderPtr->m_Rigid->userData);
			colliderPtr->m_pOwner->m_State = data->m_State;
			m_CollisionObjects.push_back(make_pair(entId, obj.second));
			cout << "Enter Entity ID :" << data->m_EntityId << endl;
		}
	}
}

void PhysicsManager::DeleteCollisionCollider(int entId)
{
	for (const auto& obj : m_pStaticColliders) {
		int Id = obj.first;
		StaticCollider* colliderPtr = obj.second;

		if (obj.first == entId)
		{
			UserData* data = static_cast<UserData*>(colliderPtr->m_Rigid->userData);
			colliderPtr->m_pOwner->m_State = data->m_State;

			auto it = std::remove_if(m_CollisionObjects.begin(), m_CollisionObjects.end(),
				[entId](const std::pair<int, StaticCollider*>& element)
				{
					cout << "Enter Entity ID :" << entId << endl;
					return element.first == entId;
				});

			m_CollisionObjects.erase(it);
		}
	}
}

DynamicCollider* PhysicsManager::GetDynamicCollider(int entId)
{
	const auto it = std::find_if(m_pDynamicColliders.begin(), m_pDynamicColliders.end(), [entId](std::pair<int, DynamicCollider*> collider)
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
				if (userData->m_CollisionType == CollisionType::PLAYER)
					player = true;
			}

			for (auto& actor : pair)
			{
				UserData* userData = static_cast<UserData*>(actor->userData);
				userData->m_State = CollisionState::ENTER;

				if (player && userData->m_CollisionType != CollisionType::PLAYER)
				{
					PhysicsManager::GetInstance()->AddCollisionColliders(userData->m_EntityId);
					player = false;
				}
			}
		}

		//// Collsion Stay
		//if (pair.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		//{
		//	auto& pair = pairHeader.actors;

		//	bool player = false;
		//	for (auto& actor : pair)
		//	{
		//		UserData* userData = static_cast<UserData*>(actor->userData);
		//		if (userData->m_CollisionType == CollisionType::PLAYER)
		//			player = true;
		//	}

		//	for (auto& actor : pair)
		//	{
		//		UserData* userData = static_cast<UserData*>(actor->userData);
		//		userData->m_State = CollisionState::STAY;

		//		if (player && userData->m_CollisionType != CollisionType::PLAYER)
		//		{
		//			player = false;
		//		}
		//	}
		//}

		// Collsion Exit
		if (pair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			auto& pair = pairHeader.actors;

			for (auto& actor : pair)
			{
				UserData* userData = static_cast<UserData*>(actor->userData);
				userData->m_State = CollisionState::EXIT;
				PhysicsManager::GetInstance()->DeleteCollisionCollider(userData->m_EntityId);
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
			PhysicsManager::GetInstance()->AddCollisionColliders(userData->m_EntityId);
		}

		if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			UserData* userData = static_cast<UserData*>(pair.triggerActor->userData);
			userData->m_State = CollisionState::EXIT;
			PhysicsManager::GetInstance()->DeleteCollisionCollider(userData->m_EntityId);
		}
	}
}

