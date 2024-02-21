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
		bool m_IsInteract = m_pOwner->get<Interactive>()->m_IsInteract;
		if (m_pOwner->has<BoxCollider>())
		{
			if (m_pOwner->get<BoxCollider>()->m_IsRaycastHit)
			{
				// ¿Ü°û¼± Ã³¸®
				if (InputM->GetKeyDown(Key::E))
				{
					m_pOwner->getWorld()->each<Interactive>([&](Entity* ent, ComponentHandle<Interactive> interactive)
						{
							if(interactive->m_DoorIndex == m_pOwner->get<Interactive>()->m_DoorIndex)
								interactive->m_IsInteract = !m_IsInteract;
						});
				}
			}
		}

		if(m_pOwner->has<Interactive>())
		{
			if (m_pOwner->get<Interactive>()->m_IsInteract)
			{
				if (m_pOwner->get<Transform>()->m_Position.GetX() >= 150.f || m_pOwner->get<Transform>()->m_Position.GetX() <= -150.f)
				{
					m_pOwner->get<Transform>()->m_Position.SetX(150.f * m_pOwner->get<Interactive>()->m_OpeningDir);
				}
				else
				{
					m_pOwner->get<Transform>()->m_Position.AddX(m_pOwner->get<Interactive>()->m_OpeningDir * OBJECT_SPEED * deltaTime);
				}
			}
			else
			{
				if (m_pOwner->get<Transform>()->m_Position.GetX() <= 50.f && m_pOwner->get<Transform>()->m_Position.GetX() >= -50.f)
				{
					m_pOwner->get<Transform>()->m_Position.SetX(50.f * m_pOwner->get<Interactive>()->m_OpeningDir);
				}
				else
				{
					m_pOwner->get<Transform>()->m_Position.AddX(-m_pOwner->get<Interactive>()->m_OpeningDir * OBJECT_SPEED * deltaTime);
				}
			}
		}
		
	}
};