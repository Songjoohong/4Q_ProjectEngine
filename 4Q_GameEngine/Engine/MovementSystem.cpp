#include "pch.h"
#include "MovementSystem.h"

#include "Movement.h"
#include "Transform.h"

void MovementSystem::Tick(World* world, float deltaTime)
{
	world->each<Transform, Movement>([&](ECS::Entity* entity, ECS::ComponentHandle<Transform> transform, ECS::ComponentHandle<Movement> movement)->void
		{
			Vector3D moveVector;
			
			Vector3D rotationVector = movement->m_UpVector * movement->m_CurrentRotation.y + movement->m_RightVector * movement->m_CurrentRotation.x;

			movement->m_RightVector = movement->m_DirectionVector.Cross(Vector3D{ 0.f,1.f,0.f }).Normalize();
			movement->m_UpVector = movement->m_RightVector.Cross(movement->m_DirectionVector).Normalize();

			if (movement->m_CurrentMoveState & 0x0001)
				moveVector -= movement->m_DirectionVector;

			if (movement->m_CurrentMoveState & 0x0010)
				moveVector += movement->m_DirectionVector;

			if (movement->m_CurrentMoveState & 0x0100)
				moveVector -= movement->m_RightVector;

			if (movement->m_CurrentMoveState & 0x1000)
				moveVector += movement->m_RightVector;

			movement->m_CurrentMoveState = 0;
			moveVector = moveVector.Normalize();
			transform->m_Position += moveVector * movement->m_Speed * deltaTime;
			transform->m_Rotation += rotationVector * movement->m_Sensitivity * deltaTime;
			movement->m_DirectionVector += rotationVector;
			movement->m_DirectionVector = movement->m_DirectionVector.Normalize();
		});
}
