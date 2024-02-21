#pragma once
#include "Camera.h"
#include "ECS.h"
#include "InputManager.h"
#include "Script.h"
#include "EntityIdentifier.h"
#include "Sprite2D.h"
#include "UI.h"

static bool m_ShowPauseSprties = false;
static bool m_IsBackToLobbyPressed = false;
static bool m_IsShutDownPressed = false;

class PauseScript : public Script
{
public:

	explicit PauseScript(Entity* ent)
		:Script(ent)
	{
	}

	virtual ~PauseScript() = default;

	virtual void Update(float delatTime) override
	{
		if (InputManager::GetInstance()->GetKeyDown(Key::ESC))
		{
			m_ShowPauseSprties = true;
		}

		// UI들 보여질 때 동안은 플레이어 키 잠금
		if (m_ShowPauseSprties)
		{
			InputManager::GetInstance()->LockKey(Key::LEFT);
			InputManager::GetInstance()->LockKey(Key::RIGHT);
			InputManager::GetInstance()->LockKey(Key::UP);
			InputManager::GetInstance()->LockKey(Key::DOWN);
			InputManager::GetInstance()->LockKey(Key::E);
			InputManager::GetInstance()->LockKey(Key::ESC);
		}

		if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "PauseSprite")
		{
			if (m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else if (!m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}

		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "ContinueSprite")
		{
			if (m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else if (!m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}

			// 계속하기 눌렀을 때 창들 다 다시 다 끄고 키 잠금 풀기
			if (m_pOwner->get<UI>()->m_UIstate == 1)
			{
				m_ShowPauseSprties = false;
				InputManager::GetInstance()->UnlockKey();
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "BackToLobbySprite")
		{
			// 이거 눌렀을 때는 예 아니요 창과 진짜 로비로 돌아갈건지 창 떠야됨
			if (m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else if (!m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}

			if (m_pOwner->get<UI>()->m_UIstate == 1)
			{
				m_ShowPauseSprties = false;
				m_IsBackToLobbyPressed = true;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "ShutDownSprite")
		{
			if (m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else if (!m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}

			if (m_pOwner->get<UI>()->m_UIstate == 1)
			{
				m_ShowPauseSprties = false;
				m_IsShutDownPressed = true;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "YesSprite")
		{
			if (m_IsBackToLobbyPressed || m_IsShutDownPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else if (!m_IsBackToLobbyPressed || !m_IsShutDownPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}

			if (m_pOwner->get<UI>()->m_UIstate == 1 && m_IsBackToLobbyPressed)
			{
				m_ShowPauseSprties = true;
				m_IsBackToLobbyPressed = false;
				m_pOwner->get<UI>()->m_GoBackToLobby = true;
			}
			else if (m_pOwner->get<UI>()->m_UIstate == 1 && m_IsShutDownPressed)
			{
				m_ShowPauseSprties = true;
				m_IsShutDownPressed = false;
				m_pOwner->get<UI>()->m_EndGame = true;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "NoSprite")
		{
			if (m_IsBackToLobbyPressed || m_IsShutDownPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else if (!m_IsBackToLobbyPressed || !m_IsShutDownPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}

			// 아니요 눌렀을 때 다시 돌아감
			if (m_pOwner->get<UI>()->m_UIstate == 1 && m_IsBackToLobbyPressed)
			{
				m_ShowPauseSprties = true;
				m_IsBackToLobbyPressed = false;
			}
			else if (m_pOwner->get<UI>()->m_UIstate == 1 && m_IsShutDownPressed)
			{
				m_ShowPauseSprties = true;
				m_IsShutDownPressed = false;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "ReallyBackToLobbySprite")
		{
			if (m_IsBackToLobbyPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else if (!m_IsBackToLobbyPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "ReallyShutDownSprite")
		{
			if (m_IsShutDownPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else if (!m_IsShutDownPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
			}
		}
	}

};