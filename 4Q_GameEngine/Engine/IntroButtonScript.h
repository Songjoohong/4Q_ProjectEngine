#pragma once
#include "ECS.h"
#include "Script.h"
#include "UI.h"
#include "EntityIdentifier.h"
#include "InputManager.h"
#include "Sprite2D.h"
static bool m_IsECButtonPressed = false;
static bool m_IsEndingCreditClosed = false;
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
			auto uiState = m_pOwner->get<UI>()->m_UIstate;

			switch (uiState)
			{
			case CLICK:
				m_IsECButtonPressed = true;
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/title_click_btn_developer.png";
				break;
			case UINONE:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/title_btn_developer.png";
				break;
			case HOVER:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/title_mouseup_btn_developer.png";
				break;
			default:
				break;
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
			}

			if (m_IsEndingCreditClosed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}
		}

		if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "ExitButton")
		{
			auto uiState = m_pOwner->get<UI>()->m_UIstate;

			switch (uiState)
			{
			case CLICK:
				m_IsECButtonPressed = true;
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/title_click_btn_end.png";
				break;
			case UINONE:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/title_btn_end.png";
				break;
			case HOVER:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/title_mouseup_btn_end.png";
				break;
			default:
				break;
			}
		}
	}

};