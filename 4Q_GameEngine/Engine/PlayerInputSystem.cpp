#include "pch.h"
#include "PlayerInputSystem.h"

#include "InputSystem.h"
#include "Movement.h"
#include "Transform.h"

void PlayerInputSystem::Tick(ECS::World* world, ECS::DefaultTickData data)
{
	Vector3D vec = { 0.f,0.f,0.f };
	if(InputSystem::GetInstance()->GetKeyState(static_cast<Key>('W')).KeyState == KeyState::STAY)
	{
		vec += {1.f, 0.f, 0.f};
	}
	if (InputSystem::GetInstance()->GetKeyState(static_cast<Key>('A')).KeyState == KeyState::STAY)
	{
		vec += {0.f, -1.f, 0.f};
	}
	if (InputSystem::GetInstance()->GetKeyState(static_cast<Key>('S')).KeyState == KeyState::STAY)
	{
		vec += {-1.f, 0.f, 0.f};
	}
	if (InputSystem::GetInstance()->GetKeyState(static_cast<Key>('D')).KeyState == KeyState::STAY)
	{
		vec += {0.f, 1.f, 0.f};
	}

	world->each<Transform, Movement>([&](ECS::Entity* entity, ECS::ComponentHandle<Transform> transform, ECS::ComponentHandle<Movement> movement)->void
		{
			movement->m_DirectionVector = vec;
		});
}
