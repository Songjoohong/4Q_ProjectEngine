#include "pch.h"
#include "TransformSystem.h"

#include "Transform.h"

void TransformSystem::Tick(World* world, ECS::DefaultTickData data)
{
	world->each<Transform>([&](Entity* ent, const ComponentHandle<Transform> transform)
		{
			const DirectX::SimpleMath::Vector4 quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(transform->m_Rotation.GetY()), DirectX::XMConvertToRadians(transform->m_Rotation.GetX()), DirectX::XMConvertToRadians(transform->m_Rotation.GetZ()));

			transform->m_RelativeMatrix = DirectX::SimpleMath::Matrix::CreateScale(transform->m_Scale.ConvertToVector3()) * DirectX::SimpleMath::Matrix::CreateFromQuaternion(quaternion) * DirectX::SimpleMath::Matrix::CreateTranslation(transform->m_Position.ConvertToVector3());
		});

	world->each<Transform>([&](const Entity* ent,const ComponentHandle<Transform> transform)
		{
			if (ent->getParent() != nullptr)
				transform->m_WorldMatrix = transform->m_RelativeMatrix.ConvertToMatrix() * ent->getParent()->get<Transform>()->m_WorldMatrix.ConvertToMatrix();
			else
				transform->m_WorldMatrix = transform->m_RelativeMatrix;
		});
}
