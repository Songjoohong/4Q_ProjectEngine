#pragma once
#include "Camera.h"
#include "ECS.h"
#include "InputManager.h"
#include "Script.h"
#include "EntityIdentifier.h"
#include "Sprite2D.h"
#include "UI.h"

static bool m_ShowPauseSprties = false;

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

		if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "PauseSprite")
		{
			if (m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else 
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "ContinueSprite")
		{
			if (m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;

			if (m_pOwner->get<UI>()->m_UIstate == 1)
			{
				m_ShowPauseSprties = false;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "BackToLobbySprite")
		{
			if (m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;

			if (m_pOwner->get<UI>()->m_UIstate == 1)
			{

			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "ShutDownSprite")
		{
			if (m_ShowPauseSprties)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;

			if (m_pOwner->get<UI>()->m_UIstate == 1)
			{

			}
		}
	}

};