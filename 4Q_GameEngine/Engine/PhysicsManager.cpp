#include "pch.h"
#include "PhysicsManager.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"

void PhysicsManager::Initialize()
{
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

	// ���� : ����� ���� ��ġ���� ������Ʈ�� �־��ֱ�
	for (auto& collider : m_pDynamicColliders)
		if (collider->m_pOwner->m_IsTrigger == false)
			collider->UpdatePhysics();
}

void PhysicsManager::ChangePxScene(ECS::World* world)
{
	// ���� : �ٲ� ���忡 PxScene�� ������ �´�.
	m_pCurrentPxScene = m_pPxScenes[world];

	/*
		���� : �߰������� �۾��ؾ���.
		 ex) �÷��̾� �Ǵ� �繰�� Scene�� �̵��ϴ� ���
		 ���� PxScene ���� removeActor�� ���� ��
		 ���� PxScene �� AddActor�� ������Ѵ�.
	*/
}
