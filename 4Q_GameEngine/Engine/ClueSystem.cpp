#include "pch.h"
#include "ClueSystem.h"

#include "Clue.h"
#include "InputManager.h"
#include "Sprite2D.h"
#include "UI.h"

void ClueSystem::Configure(World* world)
{
	world->Subscribe<Events::CluePageChange>(this);
	world->Subscribe<Events::ClueFound>(this);
}

void ClueSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void ClueSystem::Receive(World* world, const Events::CluePageChange& event)
{
	m_CurrentPage = event.page;
}

void ClueSystem::Receive(World* world, const Events::ClueFound& event)
{
	world->each<Clue, Sprite2D>([&](Entity* ent, ComponentHandle<Clue> clue, ComponentHandle<Sprite2D> sprite)
		{
			if (clue->m_Page == event.page && clue->m_Index == event.index)
				sprite->m_FileName = clue->m_FilePath;
		});
}

void ClueSystem::Tick(World* world, ECS::DefaultTickData data)
{
	if (InputM->GetKeyDown(Key::TAB))
	{
		m_IsClueOn = !m_IsClueOn;
	}

	world->each<Clue, UI>([&](Entity* ent, ComponentHandle<Clue> clue, ComponentHandle<UI> ui)
		{
			ui->m_IsShow = m_IsClueOn;

			if (m_CurrentPage != clue->m_Page)
			{
				ui->m_IsShow = false;
			}
		});
}