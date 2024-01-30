#include "pch.h"
#include "PhysicsManager.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"

void PhysicsManager::Initialize()
{
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

	// 석영 : 결과로 나온 위치값을 오브젝트로 넣어주기
	for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdatePhysics();
}

void PhysicsManager::ChangePxScene(ECS::World* world)
{
	// 석영 : 바뀐 월드에 PxScene을 가지고 온다.
	m_pCurrentPxScene = m_pPxScenes[world];

	/*
		석영 : 추가적으로 작업해야함.
		 ex) 플레이어 또는 사물이 Scene을 이동하는 경우
		 이전 PxScene 에서 removeActor를 해준 후
		 현재 PxScene 에 AddActor를 해줘야한다.
	*/
}
