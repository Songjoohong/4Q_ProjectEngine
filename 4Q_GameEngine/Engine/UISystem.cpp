#include "pch.h"
#include "UISystem.h"

#include "InputManager.h"
#include "Sprite2D.h"
#include "UI.h"

void UISystem::Tick(World* world, ECS::DefaultTickData data)
{
	world->each<UI, Sprite2D>([&](Entity* ent, ComponentHandle<UI> ui, ComponentHandle<Sprite2D> sprite)
		{
			sprite->m_IsRendered = ui->m_IsShow;
			const int mousePos[2] = { InputManager::GetInstance()->GetMousePos().x, InputManager::GetInstance()->GetMousePos().y };
			if(mousePos[0] >= sprite->m_Position[0] && mousePos[0] <= sprite->m_Position[0] + ui->m_Size[0] && mousePos[1] >= sprite->m_Position[1] && mousePos[1] <= sprite->m_Position[1] + ui->m_Size[1])
			{
				ui->m_UIstate = UISTATE::HOVER;
			}
			else
			{
				ui->m_UIstate = UISTATE::UINONE;
			}
			if (ui->m_UIstate == UISTATE::HOVER && InputManager::GetInstance()->GetMouseButton(0))
				ui->m_UIstate = UISTATE::CLICK;
		});
}
