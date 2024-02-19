#include "pch.h"
#include "GameApp.h"
#include "../D3D_Graphics/D3D_Graphics.h"
#include "../Engine/ECS.h"
#include "../Engine/WorldManager.h"
#include "ImGuizmo.h"

//Component Headers
#include "../Engine/Transform.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/Camera.h"
#include "../Engine/FreeCameraScript.h"
#include "../Engine/Light.h"
#include "../Engine/EntityIdentifier.h"
#include "../Engine/Movement.h"
#include "../Engine/StaticMesh.h"
#include "../Engine/Debug.h"
#include "../Engine/Sound.h"
#include "../Engine/RenderManager.h"
#include "../Engine/Sprite2D.h"
#include "../Engine/RigidBody.h"
#include "../Engine/UI.h"
#include "../Engine/Space.h"
#include "../Engine/DynamicText.h"

// Script Headers
#include "../Engine/SampleScript.h"
#include "../Engine/FreeCameraScript.h"
#include "../Engine/PlayerScript.h"
#include "../Engine/POVCameraScript.h"
#include "../Engine/TestUIScript.h"
#include "../Engine/DynamicTextScript.h"

// system Headers
#include "../Engine/MovementSystem.h"
#include "../Engine/TransformSystem.h"
#include "../Engine/CameraSystem.h"
#include "../Engine/RenderSystem.h"
#include "../Engine/ScriptSystem.h"
#include "../Engine/TimeManager.h"
#include "../Engine/CollisionSystem.h"
#include "../Engine/SpriteSystem.h"
#include "../Engine/DebugSystem.h"
#include "../Engine/UISystem.h"
#include "../Engine/SpaceSystem.h"

GameApp::GameApp(HINSTANCE hInstance)
	:Engine(hInstance)
{

}

GameApp::~GameApp()
{

}

bool GameApp::Initialize(UINT Width, UINT Height)
{
	const bool result = __super::Initialize(Width, Height);
	if (!result)
		return result;

	DeserializeGame("scene/ScriptTestScene.scene");

	return true;
}

void GameApp::DeserializeGame(const std::string filename)
{
	std::string fullPath = basePath + filename;

	m_CurrentWorld = ECS::World::CreateWorld(filename);
	WorldManager::GetInstance()->ChangeWorld(m_CurrentWorld);
	m_CurrentWorld->registerSystem(new ScriptSystem());
	m_CurrentWorld->registerSystem(new MovementSystem());
	m_CurrentWorld->registerSystem(new CollisionSystem());
	m_CurrentWorld->registerSystem(new TransformSystem());
	m_CurrentWorld->registerSystem(new DebugSystem());
	m_CurrentWorld->registerSystem(new CameraSystem());
	m_CurrentWorld->registerSystem(new RenderSystem());
	m_CurrentWorld->registerSystem(new SpriteSystem());
	m_CurrentWorld->registerSystem(new class UISystem);
	m_CurrentWorld->registerSystem(new SpaceSystem());

	// Deserialize
	std::ifstream inputFile(fullPath);
	json jsonObject;
	inputFile >> jsonObject;
	inputFile.close();

	for (const auto& entity : jsonObject["WorldEntities"])
	{
		for (auto it = entity.begin(); it != entity.end(); ++it)
		{
			// Entity 생성 후 정보 push
			Entity* myEntity = m_CurrentWorld->create();

			for (auto component : it.value())
			{
				std::string componentName = component.begin().key();

				if (componentName == "EntityIdentifier")
				{
					AssignComponents<EntityIdentifier>(myEntity, component["EntityIdentifier"][0]);
					//m_NameManager->AddEntityName(myEntity);
				}
				else if (componentName == "Transform")
				{
					AssignComponents<Transform>(myEntity, component["Transform"][0]);
				}
				else if (componentName == "BoxCollider")
				{
					AssignComponents<BoxCollider>(myEntity, component["BoxCollider"][0]);
				}

				else if (componentName == "Camera")
				{
					AssignComponents<Camera>(myEntity, component["Camera"][0]);
				}

				else if (componentName == "Light")
				{
					AssignComponents<Light>(myEntity, component["Light"][0]);
				}

				else if (componentName == "Movement")
				{
					AssignComponents<Movement>(myEntity, component["Movement"][0]);
				}

				else if (componentName == "StaticMesh")
				{
					//AssignComponents<StaticMesh>(myEntity, component["StaticMesh"][0]);
					std::string fileName = component["StaticMesh"][0]["m_FileName"];
					myEntity->Assign<StaticMesh>(fileName);
					myEntity->get<StaticMesh>().get().m_ComponentName = component["StaticMesh"][0]["m_ComponentName"];
					myEntity->get<StaticMesh>().get().m_FileName = component["StaticMesh"][0]["m_FileName"];
					myEntity->get<StaticMesh>().get().m_IsModelCreated = component["StaticMesh"][0]["m_IsModelCreated"];
				}
				else if (componentName == "Debug")
				{
					AssignComponents<Debug>(myEntity, component["Debug"][0]);
				}
				else if (componentName == "Sound")
				{
					AssignComponents<Sound>(myEntity, component["Sound"][0]);
				}
				else if (componentName == "RigidBody")
				{
					AssignComponents<RigidBody>(myEntity, component["RigidBody"][0]);
				}
				else if (componentName == "UI")
				{
					AssignComponents<UI>(myEntity, component["UI"][0]);
				}
				else if (componentName == "Space")
				{
					AssignComponents<Space>(myEntity, component["Space"][0]);
				}
				else if (componentName == "DynamicText")
				{
					AssignComponents<DynamicText>(myEntity, component["DynamicText"][0]);
				}
				else if (componentName == "Sprite2D")
				{
					AssignComponents<Sprite2D>(myEntity, component["Sprite2D"][0]);
				}
				else if (componentName == "Script")
				{
					if (component["Script"][0]["m_ComponentName"].get<std::string>() == "FreeCameraScript")
					{
						AssignComponents<FreeCameraScript>(myEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "SampleScript")
					{
						AssignComponents<SampleScript>(myEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "PlayerScript")
					{
						AssignComponents<PlayerScript>(myEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "POVCameraScript")
					{
						AssignComponents<POVCameraScript>(myEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "TestUIScript")
					{
						AssignComponents<TestUIScript>(myEntity, component["Script"][0]);
					}
				}
			}
		}
	}

	//부모자식 관계 설정
	for (const auto& childEntity : m_CurrentWorld->GetEntities())
	{
		for (const auto& parentEntity : m_CurrentWorld->GetEntities())
		{
			if (childEntity->get<EntityIdentifier>().get().m_HasParent == true)
			{
				if (childEntity->get<EntityIdentifier>().get().m_ParentEntityId == parentEntity->getEntityId())
				{
					SetParent(childEntity, parentEntity);
				}
			}
		}
	}

	for (const auto& entity : m_CurrentWorld->GetEntities())
	{
		if (entity->get<EntityIdentifier>()->m_EntityName == "Main Camera")
		{
			m_CurrentWorld->destroy(entity);
		}
	}
}

void GameApp::SetParent(ECS::Entity* child, ECS::Entity* parent)
{
	child->get<EntityIdentifier>().get().m_ParentEntityId = parent->getEntityId();
	child->get<EntityIdentifier>().get().m_HasParent = true;

	parent->addChild(child);
}

void GameApp::SetParentTransform(ECS::Entity* child, ECS::Entity* parent)
{
	auto matrix = child->get<Transform>().get().m_RelativeMatrix.ConvertToMatrix() * DirectX::XMMatrixInverse(nullptr, parent->get<Transform>()->m_WorldMatrix.ConvertToMatrix());

	float fTranslation[3] = { 0.0f, 0.0f, 0.0f };
	float fRotation[3] = { 0.0f, 0.0f, 0.0f };
	float fScale[3] = { 0.0f, 0.0f, 0.0f };

	ImGuizmo::DecomposeMatrixToComponents(*matrix.m, fTranslation, fRotation, fScale);

	child->get<Transform>()->m_Position = { fTranslation[0],fTranslation[1],fTranslation[2] };
	child->get<Transform>()->m_Rotation = { fRotation[0],fRotation[1],fRotation[2] };
	child->get<Transform>()->m_Scale = { fScale[0],fScale[1],fScale[2] };
}

void GameApp::Update()
{
	__super::Update();
}

void GameApp::Render()
{
	__super::Render();
}
