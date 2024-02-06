#pragma once
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
	}
};
