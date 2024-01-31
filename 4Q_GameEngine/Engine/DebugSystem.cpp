#include "pch.h"
#include "DebugSystem.h"

#include "RenderManager.h"
#include "Transform.h"
#include "../D3D_Graphics/D3D_Graphics.h"

void DebugSystem::Configure(World* world)
{
	world->Subscribe<Events::OnComponentAssigned<Debug>>(this);
	world->Subscribe<Events::OnEntityDestroyed>(this);
}

void DebugSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void DebugSystem::Receive(World* world, const Events::OnComponentAssigned<Debug>& event)
{
	RenderManager::GetInstance()->AddDebug(event.entity->getEntityId(), " ", Vector3D());
}

void DebugSystem::Receive(World* world, const Events::OnEntityDestroyed& event)
{
}

void DebugSystem::Tick(World* world, ECS::DefaultTickData data)
{
	world->each<Debug, Transform>([&](Entity* ent, ComponentHandle<Debug> debug, ComponentHandle<Transform> transform)->void
		{
			string text = "(" + to_string(transform->m_Position.GetX()) + ", " + to_string(transform->m_Position.GetY()) + ", " + to_string(transform->m_Position.GetZ()) + ")";

			RenderManager::GetInstance()->EditDebug(ent->getEntityId(), text, transform->m_Position);
		});
}
