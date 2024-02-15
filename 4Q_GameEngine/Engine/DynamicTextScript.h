#pragma once
#include "pch.h"
#include "DynamicText.h"
#include "ECS.h"
#include "InputManager.h"
#include "Script.h"

struct DynamicTextScript : public Script
{
	DynamicTextScript(Entity* ent)
		:Script(ent)
	{}
	virtual ~DynamicTextScript() override = default;
	virtual void Awake() override
	{
		m_Size = m_pOwner->get<DynamicText>()->m_Text.size();
	}

	virtual void Update(float deltaTime) override
	{
		if(InputM->GetKeyDown(Key::SPACE))
		{
			m_pOwner->get<DynamicText>()->m_CurrentTextIndex += 1;
			m_pOwner->get<DynamicText>()->m_CurrentTextIndex %= m_Size;
			m_pOwner->getWorld()->emit<Events::DynamicTextChange>({ m_pOwner });
		}

		if(InputM->GetKeyDown(Key::MBUTTON))
		{
			m_pOwner->get<DynamicText>()->m_IsTextShow = !m_pOwner->get<DynamicText>()->m_IsTextShow;
			m_pOwner->getWorld()->emit<Events::DynamicTextChange>({ m_pOwner });
		}
	}

	int m_Size = 0;
};
