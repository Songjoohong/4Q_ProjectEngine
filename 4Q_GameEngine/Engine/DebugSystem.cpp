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
	world->Subscribe<Events::OnEntityDestroyed>(this);
	world->Subscribe<Events::OnComponentAssigned<DynamicText>>(this);
	world->Subscribe<Events::BroadCastPlayer>(this);
}

void DebugSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void DebugSystem::Receive(World* world, const Events::OnComponentAssigned<Debug>& event)
{
	RenderManager::GetInstance()->AddText(event.entity->getEntityId(), "", Vector3D());
}

void DebugSystem::Receive(World* world, const Events::OnEntityDestroyed& event)
{
	if(event.entity->has<Debug>())
		RenderManager::GetInstance()->DeleteText(static_cast<int>(event.entity->getEntityId()));

	if (event.entity->has<DynamicText>())
		RenderManager::GetInstance()->DeleteText(static_cast<int>(event.entity->getEntityId()));
}

void DebugSystem::Receive(World* world, const Events::OnComponentAssigned<DynamicText>& event)
{
	RenderManager::GetInstance()->AddText(event.entity->getEntityId(), "", Vector3D());
}

void DebugSystem::Receive(World* world, const Events::BroadCastPlayer& event)
{
	m_Player = event.playerEntity;
}


void DebugSystem::Tick(World* world, ECS::DefaultTickData data)
{
	world->each<Debug, Transform>([&](Entity* ent, ComponentHandle<Debug> debug, ComponentHandle<Transform> transform)->void
		{
			string text = "(" + to_string(transform->m_Position.GetX()) + ", " + to_string(transform->m_Position.GetY()) + ", " + to_string(transform->m_Position.GetZ()) + ")";

			RenderManager::GetInstance()->EditText(ent->getEntityId(), text, transform->m_Position);
		});

	world->each<DynamicText, Transform>([&](Entity* ent, ComponentHandle<DynamicText> dynamicText, ComponentHandle<Transform> transform)->void
		{
			const Vector3 playerPos = m_Player->get<Transform>()->m_WorldMatrix.ConvertToMatrix().Translation();
			Vector3D textPos = transform->m_Position;
			Vector3D pos =  transform->m_Position * 0.5 + playerPos * 0.5f;
			string text = dynamicText->m_Text[dynamicText->m_CurrentTextIndex];
			RenderManager::GetInstance()->EditText(ent->getEntityId(), text, pos);
		});
}
