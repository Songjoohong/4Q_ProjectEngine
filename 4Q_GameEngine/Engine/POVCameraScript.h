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

	virtual void Update(float deltaTime) override
	{
		Vector3D direction = m_pOwner->get<Movement>()->m_DirectionVector;
		if(direction.GetY() > 0.8f)
		{
			direction.SetX(0.8f);
			m_pOwner->get<Movement>()->m_CurrentRotation[1] = min(InputM->GetMouseMove().y, 0.f);
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

		Vector3D pos=m_pOwner->get<Transform>()->m_Position;
		PxVec3 pxPos = { pos.GetX(),pos.GetY(),pos.GetZ() };
		PxVec3 dir = { direction.GetX(),direction.GetY(),direction.GetZ() };
		dir.normalize();
		PhysicsManager::GetInstance()->RayCast(pxPos, dir);
	}
};
