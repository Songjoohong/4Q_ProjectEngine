#include "pch.h"
#include "TransformSystem.h"

#include "Transform.h"

void TransformSystem::Configure(ECS::World* world)
{
	world->Subscribe<ECS::Events::OnComponentAssigned<Transform>>(this);
}

void TransformSystem::Deconfigure(ECS::World* world)
{
	world->UnsubscribeAll(this);
}

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

void TransformSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<Transform>& event)
{
	const DirectX::SimpleMath::Vector4 quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(event.component->m_Rotation.GetY()), DirectX::XMConvertToRadians(event.component->m_Rotation.GetX()), DirectX::XMConvertToRadians(event.component->m_Rotation.GetZ()));

	event.component->m_RelativeMatrix = DirectX::SimpleMath::Matrix::CreateScale(event.component->m_Scale.ConvertToVector3()) * DirectX::SimpleMath::Matrix::CreateFromQuaternion(quaternion) * DirectX::SimpleMath::Matrix::CreateTranslation(event.component->m_Position.ConvertToVector3());

	if (event.entity->getParent() != nullptr)
		event.component->m_WorldMatrix = event.component->m_RelativeMatrix.ConvertToMatrix() * event.entity->getParent()->get<Transform>()->m_WorldMatrix.ConvertToMatrix();
	else
		event.component->m_WorldMatrix = event.component->m_RelativeMatrix;
	
}
