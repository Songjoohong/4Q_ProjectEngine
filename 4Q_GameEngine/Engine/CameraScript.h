#pragma once
#include "ECS.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"

class CameraScript : public Script
{
public:
	CameraScript(Entity* ent)
		:Script(ent)
	{}
	virtual ~CameraScript() override = default;

	virtual void Update(float deltaTime) override
	{
		//m_pOwner->get<Movement>()->m_CurrentRotation[0] = Input->GetMouseMove().x;
		m_pOwner->get<Movement>()->m_CurrentRotation[1] = Input->GetMouseMove().y;
	}
};
