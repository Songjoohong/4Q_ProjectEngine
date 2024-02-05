#include "pch.h"
#include "CameraSystem.h"

#include "Camera.h"
#include "RenderManager.h"
#include "Transform.h"

void CameraSystem::Tick(World* world, ECS::DefaultTickData data)
{
	world->each<Camera, Transform>([&](Entity* ent, ComponentHandle<Camera> camera, ComponentHandle<Transform> transform)
		{
			RenderManager::GetInstance()->SetCamera(transform->m_WorldMatrix.ConvertToMatrix());
		});
}
