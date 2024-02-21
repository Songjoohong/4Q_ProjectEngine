#pragma once
#include "ECS.h"
#include "Script.h"
#include "UI.h"
#include "EntityIdentifier.h"
#include "InputManager.h"
#include "Sprite2D.h"
struct IntroButtonScript : public Script
{
	IntroButtonScript(Entity* ent)
		:Script(ent)
	{}
	virtual ~IntroButtonScript() = default;

	virtual void Update(float deltaTime) override
	{
		if (InputManager::GetInstance()->GetKeyDown(Key::Enter))
		{
			m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}

		if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "EndingCreditButton")
		{
			if (m_pOwner->get<UI>()->m_UIstate == 1)
			{
				m_IsECButtonPressed = true;
			}
		}

		if (m_IsECButtonPressed)
		{
			if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "EndingCreditSprite")
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
		}
	}

private:
	bool m_IsECButtonPressed = false;
};