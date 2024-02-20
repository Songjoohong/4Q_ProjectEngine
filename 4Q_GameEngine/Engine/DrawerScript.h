#pragma once
#include "BoxCollider.h"
#include "InputManager.h"
#include "Script.h"
#include "Transform.h"

struct DrawerScript : public Script
{
	DrawerScript(Entity* ent)
		:Script(ent)
	{
	}
	virtual ~DrawerScript() override = default;

	virtual void Update(float deltaTime) override
	{
		if (m_pOwner->get<BoxCollider>()->m_IsRaycastHit)
		{
			// �ܰ��� ó��
			if(InputM->GetKeyDown(Key::E))
			{
				m_IsInteract = !m_IsInteract;
			}
		}

		if(m_IsInteract)
		{
			if(m_pOwner->get<Transform>()->m_Position.GetZ() <= -35.f)
			{
				m_pOwner->get<Transform>()->m_Position.SetZ(-35.f);
			}
			else
			{
				m_pOwner->get<Transform>()->m_Position.SetZ(m_pOwner->get<Transform>()->m_Position.GetZ() - 10.f * deltaTime);
			}
		}
		else
		{
			if (m_pOwner->get<Transform>()->m_Position.GetZ() >= 0.f)
			{
				m_pOwner->get<Transform>()->m_Position.SetZ(0.f);
			}
			else
			{
				m_pOwner->get<Transform>()->m_Position.SetZ(m_pOwner->get<Transform>()->m_Position.GetZ() + 10.f * deltaTime);
			}
		}
	}

	bool m_IsInteract = false;
};
