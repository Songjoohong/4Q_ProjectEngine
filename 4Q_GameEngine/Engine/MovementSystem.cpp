#include "pch.h"
#include "MovementSystem.h"

#include "Movement.h"
#include "Transform.h"

void MovementSystem::Tick(World* world, float deltaTime)
{
	world->each<Transform, Movement>([&](ECS::Entity* entity, ECS::ComponentHandle<Transform> transform, ECS::ComponentHandle<Movement> movement)->void
		{
			Vector3D moveVector;

			transform->m_Rotation = { transform->m_Rotation.GetX() + movement->m_CurrentRotation.x * movement->m_Sensitivity, transform->m_Rotation.GetY() + movement->m_CurrentRotation.y * movement->m_Sensitivity, transform->m_Rotation.GetZ() };

			movement->m_RightVector = Vector3D{ transform->m_WorldMatrix.Right().x, transform->m_WorldMatrix.Right().y, transform->m_WorldMatrix.Right().z }.Normalize();
			movement->m_DirectionVector = Vector3D{ transform->m_WorldMatrix.Forward().x, transform->m_WorldMatrix.Forward().y , transform->m_WorldMatrix.Forward().z }.Normalize();

			if (movement->m_CurrentMoveState & 0x0001)
				moveVector += movement->m_DirectionVector;

			if (movement->m_CurrentMoveState & 0x0010)
				moveVector -= movement->m_DirectionVector;

			if (movement->m_CurrentMoveState & 0x0100)
				moveVector += movement->m_RightVector;

			if (movement->m_CurrentMoveState & 0x1000)
				moveVector -= movement->m_RightVector;

			// 이동 설정
			movement->m_CurrentMoveState = 0;
			moveVector = moveVector.Normalize();
			transform->m_Position += moveVector * movement->m_Speed * deltaTime;			
		});
}
