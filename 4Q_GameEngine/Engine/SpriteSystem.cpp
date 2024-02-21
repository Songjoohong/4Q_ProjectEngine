#include "pch.h"
#include "SpriteSystem.h"

#include <imgui.h>

#include "RenderManager.h"

void SpriteSystem::Configure(World* world)
{
	world->Subscribe<Events::OnComponentAssigned<Sprite2D>>(this);
	world->Subscribe<Events::OnEntityDestroyed>(this);
}

void SpriteSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void SpriteSystem::Receive(World* world, const Events::OnComponentAssigned<Sprite2D>& event)
{
	RenderManager::GetInstance()->AddSprite(world, event.entity->getEntityId(), event.component->m_FileName, { event.component->m_Position[0], event.component->m_Position[1] }, event.component->m_Layer);
}

void SpriteSystem::Receive(World* world, const Events::OnEntityDestroyed& event)
{
	if (event.entity->has<Sprite2D>())
		RenderManager::GetInstance()->DeleteSprite(static_cast<int>(event.entity->getEntityId()));
}

void SpriteSystem::Tick(World* world, ECS::DefaultTickData data)
{
	world->each<Sprite2D>([&](Entity* ent, ComponentHandle<Sprite2D> sprite2D)->void
		{
			RenderManager::GetInstance()->EditSprite(ent->getEntityId(), sprite2D.get());
		});
}
