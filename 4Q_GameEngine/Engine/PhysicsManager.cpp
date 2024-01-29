#include "pch.h"
#include "PhysicsManager.h"
#include "..\D3D_PhysX\D3D_PhysX.h"
#include "..\D3D_Graphics\StaticModel.h"


void PhysicsManager::Initialize()
{
	m_Physics = m_Physics->GetInstance();
	m_Physics->Initialize();
}

void PhysicsManager::Update()
{
	m_Physics->Update();
}
