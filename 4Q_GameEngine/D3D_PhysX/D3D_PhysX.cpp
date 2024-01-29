#include "pch.h"
#include "D3D_PhysX.h"

#include "StaticRigidInstance.h"
#include "DynamicRigidInstance.h"

#include "..\D3D_Graphics\Model.h"

Physics* Physics::m_pInstance = nullptr;
Physics* Physics::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new Physics;
	}

	return m_pInstance;
}

void Physics::Initialize()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
	assert(!m_pFoundation);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, 0);

	// 씬 만들어주기
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	m_pDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_pScene = m_pPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.f);
}

void Physics::Update()
{
	StepPhysics();
}


void Physics::StepPhysics()
{
	m_pScene->simulate(1.f / 60.f);
	m_pScene->fetchResults(true);
}

void Physics::AddStaticActor(Model* model)
{
	
}

void Physics::AddDynamicActor(Model* model)
{
}
