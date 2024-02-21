#pragma once
#include "ECS.h"
#include "Script.h"
#include "UI.h"
#include "EntityIdentifier.h"
#include "InputManager.h"
#include "Sprite2D.h"
static bool m_IsECButtonPressed = false;
static bool m_IsEndingCreditClosed = false;
static bool m_IsEndingCreditOpended = false;
struct IntroButtonScript : public Script
{
	IntroButtonScript(Entity* ent)
		:Script(ent)
	{}
	virtual ~IntroButtonScript() = default;

	virtual void Update(float deltaTime) override
	{
		// EndingCredit화면일 때 키 입력막아야함...
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

		if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "EndingCreditSprite")
		{
			if (m_IsECButtonPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			if (m_IsEndingCreditClosed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
				m_IsEndingCreditClosed = false;
				InputManager::GetInstance()->UnlockKey();
			}
		}

		if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "EndingCreditCloseButton")
		{
			if (m_IsECButtonPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
				m_IsECButtonPressed = false;
				InputManager::GetInstance()->LockKey(Key::Enter);
			}

			if (m_pOwner->get<UI>()->m_UIstate == 1)
			{
				m_IsEndingCreditClosed = true;
				m_IsEndingCreditOpended = false;
			}
			if (m_IsEndingCreditClosed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}
		}
	}

private:

};