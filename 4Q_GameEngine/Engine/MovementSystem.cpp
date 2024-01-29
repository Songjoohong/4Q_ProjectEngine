#include "pch.h"
#include "MovementSystem.h"

#include "Movement.h"
#include "Transform.h"

void MovementSystem:: Tick(World* world, float deltaTime)
{
	world->each<Transform, Movement>([&](ECS::Entity* entity, ECS::ComponentHandle<Transform> transform, ECS::ComponentHandle<Movement> movement)->void
	{
			transform->m_Position += movement->m_DirectionVector * movement->m_Speed * deltaTime;
	});
}
