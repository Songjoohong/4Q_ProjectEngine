#include "pch.h"
#include "MovementSystem.h"

#include "Movement.h"
#include "Transform.h"

void MovementSystem::Tick(World* world, float deltaTime)
{
	world->each<Transform, Movement>([&](ECS::Entity* entity, ECS::ComponentHandle<Transform> transform, ECS::ComponentHandle<Movement> movement)->void
		{
			Vector3D moveVector;
			DirectX::SimpleMath::Matrix worldMatrix = transform->m_WorldMatrix.ConvertToMatrix();
			Vector3D deltaRotation = { movement->m_CurrentRotation[1] * movement->m_Sensitivity, movement->m_CurrentRotation[0] * movement->m_Sensitivity, 0.f };

			movement->m_CurrentRotation[0] = 0;
			movement->m_CurrentRotation[1] = 0;

			transform->m_Rotation += deltaRotation;

			movement->m_RightVector = Vector3D{ worldMatrix.Right().x, worldMatrix.Right().y, worldMatrix.Right().z }.Normalize();
			movement->m_DirectionVector = Vector3D{ worldMatrix.Forward().x, worldMatrix.Forward().y , worldMatrix.Forward().z }.Normalize();

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
