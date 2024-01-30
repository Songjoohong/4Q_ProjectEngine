#include "pch.h"
#include "SoundSystem.h"

#include "SoundManager.h"

void SoundSystem::Configure(World* world)
{

	world->Subscribe<Events::OnComponentAssigned<Sound>>(this);
	world->Subscribe<Events::OnEntityDestroyed>(this);

}

void SoundSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void SoundSystem::Receive(World* world, const Events::OnComponentAssigned<Sound>& event)
{
	SoundManager::GetInstance()->CreateSound(event.component->m_FileName, event.component->m_IsLoop);
	SoundManager::GetInstance()->PlayBackSound(event.component->m_FileName);
}

void SoundSystem::Receive(World* world, const Events::OnEntityDestroyed& event)
{

}

void SoundSystem::Tick(World* world, ECS::DefaultTickData data)
{
	world->each<Sound>([&](Entity* ent, ComponentHandle<Sound> sound)->void
		{
			SoundManager::GetInstance()->SetPaused(sound->m_FileName, sound->m_IsPlaying);
			SoundManager::GetInstance()->SetVolume(sound->m_FileName, sound->m_Volume);
		});
}

