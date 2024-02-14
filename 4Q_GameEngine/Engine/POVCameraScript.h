#pragma once
#include "ECS.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"
#include "Transform.h"

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
	}
};
