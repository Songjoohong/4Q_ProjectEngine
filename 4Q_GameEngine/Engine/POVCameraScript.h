#pragma once
#include "ECS.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"
#include "Transform.h"

//Test
#include "PhysicsManager.h"

class POVCameraScript : public Script
{
public:
	POVCameraScript(Entity* ent)
		:Script(ent)
	{}
	virtual ~POVCameraScript() override = default;

	float m_ElapsedTime = 0.f;
	float m_InitialY = 0.f;

	virtual void Awake() override
	{

		if(m_pOwner->has<Transform>())
			m_InitialY = m_pOwner->get<Transform>()->m_Position.GetY();
	}

	virtual void Update(float deltaTime) override
	{
		Vector3D direction = m_pOwner->get<Movement>()->m_DirectionVector;
		if(direction.GetY() > 0.8f)
		{
			direction.SetX(0.8f);
			m_pOwner->get<Movement>()->m_CurrentRotation[1] = std::min<float>(InputM->GetMouseMove().y, 0.f);
		}
		else if(direction.GetY() < -0.8f)
		{
			direction.SetX(-0.8f);
			m_pOwner->get<Movement>()->m_CurrentRotation[1] = max(InputM->GetMouseMove().y, 0.f);
		}
		else
		{
			m_pOwner->get<Movement>()->m_CurrentRotation[1] = InputM->GetMouseMove().y;
		}

		if(InputM->GetKey(Key::UP) || InputM->GetKey(Key::LEFT) || InputM->GetKey(Key::RIGHT) || InputM->GetKey(Key::DOWN))
		{
			m_ElapsedTime += deltaTime;
			m_pOwner->get<Transform>()->m_Position.AddY(sin(m_ElapsedTime * 10.f) * deltaTime * 15.f);
		}
		else
		{
			m_pOwner->get<Transform>()->m_Position.SetY(m_InitialY * 0.01f + m_pOwner->get<Transform>()->m_Position.GetY() * 0.99f);
			m_ElapsedTime = 0;
		}
	/*	Vector3D size = m_pOwner->m_parent->get<BoxCollider>()->m_Size;
		Vector3D pos=m_pOwner->m_parent->get<Transform>()->m_Position;
		Vector3D dir = m_pOwner->get<Movement>()->m_DirectionVector;
		
		PxVec3 pxPos = { pos.GetX(),pos.GetY(),pos.GetZ()};
		PxVec3 pxDir = { dir.GetX(),dir.GetY(),dir.GetZ() };
		float r = sqrt(pow(size.GetX(), 2.f) + pow(size.GetY(), 2.f));
		pxDir.normalize();
		
		pxPos = pxPos + (pxDir * r) + (pxDir);
		PhysicsManager::GetInstance()->RayCast(pxPos, pxDir);*/
	}
};
