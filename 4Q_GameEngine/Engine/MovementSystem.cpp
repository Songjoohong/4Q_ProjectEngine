#include "pch.h"
#include "MovementSystem.h"

#include "DynamicCollider.h"
#include "Movement.h"
#include "PhysicsManager.h"
#include "RigidBody.h"
#include "Transform.h"

void MovementSystem::Tick(World* world, float deltaTime)
{
	world->each<Transform, Movement>([&](ECS::Entity* entity, ECS::ComponentHandle<Transform> transform, ECS::ComponentHandle<Movement> movement)->void
		{
			Vector3D moveVector;
			const DirectX::SimpleMath::Matrix worldMatrix = transform->m_WorldMatrix.ConvertToMatrix();

			Vector3D deltaRotation = { movement->m_CurrentRotation[1] * movement->m_Sensitivity, movement->m_CurrentRotation[0] * movement->m_Sensitivity, 0.f };

			movement->m_CurrentRotation[0] = 0;
			movement->m_CurrentRotation[1] = 0;

			if (transform->m_FreezeRotationX)
				deltaRotation.SetX(0.f);
			if (transform->m_FreezeRotationY)
				deltaRotation.SetY(0.f);
			if (transform->m_FreezeRotationZ)
				deltaRotation.SetZ(0.f);

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

			if (movement->m_CurrentMoveState & 0x10000)
				moveVector += Vector3D{ 0.f,1.f,0.f };

			if (movement->m_CurrentMoveState & 0x100000)
				moveVector -= Vector3D{ 0.f,1.f,0.f };

			movement->m_CurrentMoveState = 0;
			moveVector = moveVector.Normalize();
			// 이동 설정
			if (entity->has<RigidBody>())
			{
				DynamicCollider* player = PhysicsManager::GetInstance()->GetPlayerCollider();
				if (player != nullptr)
					player->AddForce({ moveVector.GetX(), 0.f, moveVector.GetZ()});
			}
			else
			{
				transform->m_Position += moveVector * movement->m_Speed * deltaTime;
			}

		});
}
