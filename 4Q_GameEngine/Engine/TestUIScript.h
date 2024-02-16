#pragma once
#include "InputManager.h"
#include "Script.h"
#include "UI.h"

class TestUIScript : public Script
{
public:
	TestUIScript() = default;;
	TestUIScript(Entity* ent)
		: Script(ent)
	{}
	virtual ~TestUIScript() = default;

	virtual void Update(float deltaTime) override
	{
		if(m_pOwner->has<UI>())
		{
			if (m_pOwner->get<UI>()->m_UIstate == UISTATE::HOVER)
				std::cout << "UI hover" << std::endl;
			if (m_pOwner->get<UI>()->m_UIstate == UISTATE::CLICK)
				std::cout << "UI click" << std::endl;
		}

		if(InputM->GetKeyDown(Key::F6))
		{
			m_pOwner->getWorld()->emit<Events::ClueFound>({ 1,1 });
		}
		if (InputM->GetKeyDown(Key::F7))
		{
			m_pOwner->getWorld()->emit<Events::ClueFound>({ 1,2 });
		}
		if (InputM->GetKeyDown(Key::F8))
		{
			m_pOwner->getWorld()->emit<Events::ClueFound>({ 1,3 });
		}

		if (InputM->GetKeyDown(Key::F1))
		{
			m_pOwner->getWorld()->emit<Events::CluePageChange>({ 2 });
		}
		if (InputM->GetKeyDown(Key::F2))
		{
			m_pOwner->getWorld()->emit<Events::CluePageChange>({ 1 });
		}

		if(InputM->GetKeyDown(Key::TAB))
		{
			m_pOwner->get<UI>()->m_IsShow = !m_pOwner->get<UI>()->m_IsShow;
		}
	}
};
