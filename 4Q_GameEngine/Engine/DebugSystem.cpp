#include "pch.h"
#include "DebugSystem.h"

#include "DynamicText.h"
#include "PlayerScript.h"
#include "RenderManager.h"
#include "Transform.h"
#include "../D3D_Graphics/D3D_Graphics.h"

void DebugSystem::Configure(World* world)
{
	world->Subscribe<Events::OnComponentAssigned<Debug>>(this);
	world->Subscribe<Events::OnComponentAssigned<DynamicText>>(this);
	world->Subscribe<Events::OnEntityDestroyed>(this);
	world->Subscribe<Events::DynamicTextChange>(this);
}

void DebugSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void DebugSystem::Receive(World* world, const Events::OnComponentAssigned<Debug>& event)
{
	RenderManager::GetInstance()->AddText(event.entity->getEntityId(), "", Vector3D());
}

void DebugSystem::Receive(World* world, const Events::OnComponentAssigned<DynamicText>& event)
{
	RenderManager::GetInstance()->AddDynamicText(event.entity->getEntityId(), event.component->m_Text);
}

void DebugSystem::Receive(World* world, const Events::DynamicTextChange& event)
{
	auto component = event.entity->get<DynamicText>();
	RenderManager::GetInstance()->EditDynamicText(event.entity->getEntityId(), component->m_CurrentTextIndex, component->m_IsTextShow );
}

void DebugSystem::Receive(World* world, const Events::OnEntityDestroyed& event)
{
	if(event.entity->has<Debug>())
		RenderManager::GetInstance()->DeleteText(event.entity->getEntityId());

	if (event.entity->has<DynamicText>())
		RenderManager::GetInstance()->DeleteDynamicText(event.entity->getEntityId());
}

void DebugSystem::Tick(World* world, ECS::DefaultTickData data)
{
	world->each<Debug, Transform>([&](Entity* ent, ComponentHandle<Debug> debug, ComponentHandle<Transform> transform)->void
		{
			string text = "(" + to_string(transform->m_Position.GetX()) + ", " + to_string(transform->m_Position.GetY()) + ", " + to_string(transform->m_Position.GetZ()) + ")";

			RenderManager::GetInstance()->EditText(ent->getEntityId(), text, transform->m_Position);
		});
}
