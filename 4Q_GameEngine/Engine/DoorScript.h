#pragma once
#include "BoxCollider.h"
#include "ECS.h"
#include "InputManager.h"
#include "Interactive.h"
#include "Script.h"
#include "Transform.h"

struct DoorScript : public Script
{
	DoorScript(Entity* ent)
		:Script(ent)
	{}
	virtual ~DoorScript() = default;

	virtual void Update(float deltaTime) override
	{
		if (m_pOwner->has<BoxCollider>())
		{
			auto isRayCast = m_pOwner->get<BoxCollider>()->m_IsRaycastHit;
			if (m_pOwner->get<BoxCollider>()->m_IsRaycastHit)
			{
				// ¿Ü°û¼± Ã³¸®
				if (InputM->GetKeyDown(Key::E))
				{
					m_pOwner->get<Interactive>()->m_IsInteract = !m_pOwner->get<Interactive>()->m_IsInteract;
				}
			}
		}


		if (m_pOwner->get<Interactive>()->m_IsInteract)
		{
			if (m_pOwner->get<Transform>()->m_Position.GetZ() <= 150.f * m_pOwner->get<Interactive>()->m_OpeningDir)
			{
				m_pOwner->get<Transform>()->m_Position.SetZ(150.f * m_pOwner->get<Interactive>()->m_OpeningDir);
			}
			else
			{
				m_pOwner->get<Transform>()->m_Position.SetZ(m_pOwner->get<Transform>()->m_Position.GetZ() - OBJECT_SPEED * deltaTime);
			}
		}
		else
		{
			if (m_pOwner->get<Transform>()->m_Position.GetZ() >= 90.f * m_pOwner->get<Interactive>()->m_OpeningDir)
			{
				m_pOwner->get<Transform>()->m_Position.SetZ(90.f * m_pOwner->get<Interactive>()->m_OpeningDir);
			}
			else
			{
				m_pOwner->get<Transform>()->m_Position.SetZ(m_pOwner->get<Transform>()->m_Position.GetZ() + OBJECT_SPEED * deltaTime);
			}
		}
	}
};