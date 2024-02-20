#pragma once
#include "Camera.h"
#include "ECS.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"
#include "Transform.h"
#include "EntityIdentifier.h"
class IntroDoorScript : public Script
{
public:
	bool m_IsCurCameraWork = false;
	bool m_IsPrevCameraWork = false;

	explicit IntroDoorScript(Entity* ent)
		:Script(ent)
	{
	}

	virtual ~IntroDoorScript() override = default;

	virtual void Update(float deltaTime) override
	{
		if (InputManager::GetInstance()->GetKeyDown(Key::SPACE))
		{
			m_IsRotating = true;
		}

		if(m_IsRotating)
		{
			if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "hanok_door_R")
			{
				if (m_pOwner->get<Transform>()->m_Rotation.GetY() > -130.0f) {
					m_pOwner->get<Transform>()->m_Rotation.SetY(m_pOwner->get<Transform>()->m_Rotation.GetY() - 80.0f * deltaTime);
				}
				else
					m_IsRotating = false;
			}
			else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "hanok_door_L")
			{
				if (m_pOwner->get<Transform>()->m_Rotation.GetY() < 130.0f) {
					m_pOwner->get<Transform>()->m_Rotation.SetY(m_pOwner->get<Transform>()->m_Rotation.GetY() + 80.0f * deltaTime);
				}
			}
		}
	}

private:	
	bool m_IsRotating = false;
};