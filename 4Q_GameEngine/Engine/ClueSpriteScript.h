#pragma once
#pragma once
#include "Camera.h"
#include "ECS.h"
#include "InputManager.h"
#include "Script.h"
#include "EntityIdentifier.h"
#include "Sprite2D.h"
#include "UI.h"

static bool m_IsShowClue = false;
static bool m_FrontDoorPressed = false;
static bool m_LivingRoomPressed = false;
static bool m_BookRoomPressed = false;
static bool m_StoragePressed = false;
static bool m_LastRoomPressed = false;

class ClueSpriteScript : public Script
{
public:

	explicit ClueSpriteScript(Entity* ent)
		:Script(ent)
	{
	}

	virtual ~ClueSpriteScript() = default;

	virtual void Update(float delatTime) override
	{
		if(InputManager::GetInstance()->GetKeyDown(Key::TAB))
		{
			m_IsShowClue = !m_IsShowClue;
		}

		if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "ClueNoteSprite")
		{
			if (m_IsShowClue)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "FrontDoorSprite")
		{
			if (m_IsShowClue)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			auto uiState = m_pOwner->get<UI>()->m_UIstate;

			switch (uiState)
			{
			case CLICK:
				m_FrontDoorPressed = true;
				m_LivingRoomPressed = false;
				m_BookRoomPressed = false;
				m_StoragePressed = false;
				m_LastRoomPressed = false;
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_click_btn_01.png";
				break;
			case UINONE:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_btn_01.png";
				break;
			case HOVER:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_mouseup_btn_01.png";
				break;
			default:
				break;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "LivingRoomSprite")
		{
			if (m_IsShowClue)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}

			auto uiState = m_pOwner->get<UI>()->m_UIstate;

			switch (uiState)
			{
			case CLICK:
				m_LivingRoomPressed = true;
				m_FrontDoorPressed = false;
				m_BookRoomPressed = false;
				m_StoragePressed = false;
				m_LastRoomPressed = false;
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_click_btn_02.png";
				break;
			case UINONE:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_btn_02.png";
				break;
			case HOVER:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_mouseup_btn_02.png";
				break;
			default:
				break;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "BookRoomSprite")
		{
			if (m_IsShowClue)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}

			auto uiState = m_pOwner->get<UI>()->m_UIstate;

			switch (uiState)
			{
			case CLICK:
				m_BookRoomPressed = true;
				m_LivingRoomPressed = false;
				m_FrontDoorPressed = false;
				m_StoragePressed = false;
				m_LastRoomPressed = false;
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_click_btn_03.png";
				break;
			case UINONE:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_btn_03.png";
				break;
			case HOVER:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_mouseup_btn_03.png";
				break;
			default:
				break;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "ItemStorageSprite")
		{
			if (m_IsShowClue)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}

			auto uiState = m_pOwner->get<UI>()->m_UIstate;

			switch (uiState)
			{
			case CLICK:
				m_StoragePressed = true;
				m_BookRoomPressed = false;
				m_LivingRoomPressed = false;
				m_FrontDoorPressed = false;
				m_LastRoomPressed = false;
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_click_btn_04.png";
				break;
			case UINONE:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_btn_04.png";
				break;
			case HOVER:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_mouseup_btn_04.png";
				break;
			default:
				break;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "LastRoomSprite")
		{
			if (m_IsShowClue)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}

			auto uiState = m_pOwner->get<UI>()->m_UIstate;

			switch (uiState)
			{
			case CLICK:
				m_LastRoomPressed = true;
				m_StoragePressed = false;
				m_BookRoomPressed = false;
				m_LivingRoomPressed = false;
				m_FrontDoorPressed = false;
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_click_btn_05.png";
				break;
			case UINONE:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_btn_05.png";
				break;
			case HOVER:
				m_pOwner->get<Sprite2D>()->m_FileName = "../Resource/UI/cluenote_mouseup_btn_04.png";
				break;
			default:
				break;
			}
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "DoorFirstClueSprite")
		{
			if (m_FrontDoorPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "DoorSecondClueSprite")
		{
			if (m_FrontDoorPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "DoorThirdClueSprite")
		{
			if (m_FrontDoorPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "LivingRoomFirstClueSprite")
		{
			if (m_LivingRoomPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "LibraryFirstClueSprite")
		{
			if (m_BookRoomPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "LibrarySecondClueSprite")
		{
			if (m_BookRoomPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "DepositoryFirstClueSprite")
		{
			if (m_StoragePressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "DepositorySecondClueSprite")
		{
			if (m_StoragePressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "RedRoomFirstClueSprite")
		{
			if (m_LastRoomPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
		else if (m_pOwner->get<EntityIdentifier>()->m_EntityName == "RedRoomSecondClueSprite")
		{
			if (m_LastRoomPressed)
			{
				m_pOwner->get<Sprite2D>()->m_IsRendered = true;
			}
			else
				m_pOwner->get<Sprite2D>()->m_IsRendered = false;
		}
	}

};