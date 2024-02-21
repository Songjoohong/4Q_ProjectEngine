#include "pch.h"
#include "GameApp.h"
#include "../D3D_Graphics/D3D_Graphics.h"
#include "../Engine/ECS.h"
#include "../Engine/WorldManager.h"
#include "ImGuizmo.h"
#include <cassert>
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
#include "../Engine/PlayerInformation.h"
#include "../Engine/Interactive.h"

// Script Headers
#include "../Engine/SampleScript.h"
#include "../Engine/FreeCameraScript.h"
#include "../Engine/PlayerScript.h"
#include "../Engine/POVCameraScript.h"
#include "../Engine/TestUIScript.h"
#include "../Engine/DynamicTextScript.h"
#include "../Engine/IntroCameraScript.h"
#include "../Engine/OutroScript.h"
#include "../Engine/DrawerScript.h"
#include "../Engine/IntroDoorScript.h"
#include "../Engine/DoorScript.h"
#include "../Engine/IntroButtonScript.h"

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
#include "../Engine/DrawerScript.h"
#include "../Engine/UISystem.h"
#include "../Engine/SpaceSystem.h"
#include "../Engine/EventSystem.h"



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

	//m_IntroWorld = DeserializeGame("");


	m_GameWorld = DeserializeGame("scene/ObjTest.scene");
	//m_OutroWorld = DeserializeGame("");

	WorldManager::GetInstance()->ChangeWorld(m_GameWorld);
	//WorldManager::GetInstance()->GetCurrentWorld()->emit<Events::SpaceAssemble>({ 1,2,0,0 });
	Entity* ent = WorldManager::GetInstance()->GetCurrentWorld()->create();
	ent->Assign<EntityIdentifier>(ent->getEntityId(), "Camera");
	ent->Assign<Transform>(Vector3D(0.f, 10.f, 0.f), Vector3D{ 0.f,0.f,0.f });
	ent->Assign<Debug>();
	ent->Assign<Camera>();
	ent->Assign<FreeCameraScript>(ent);
	ent->Assign<Movement>();

	return true;
}

ECS::World* GameApp::DeserializeGame(const std::string filename)
{
	ECS::World* world = ECS::World::CreateWorld(filename);

	world->registerSystem(new ScriptSystem());
	world->registerSystem(new MovementSystem());
	world->registerSystem(new CollisionSystem());
	world->registerSystem(new TransformSystem());
	world->registerSystem(new DebugSystem());
	world->registerSystem(new CameraSystem());
	world->registerSystem(new RenderSystem());
	world->registerSystem(new SpriteSystem());
	world->registerSystem(new class UISystem);
	world->registerSystem(new SpaceSystem());
	world->registerSystem(new EventSystem());

	std::string fullPath = basePath + filename;

	// Deserialize
	std::ifstream inputFile(fullPath);
	json jsonObject;
	inputFile >> jsonObject;
	inputFile.close();

	for (const auto& entity : jsonObject["WorldEntities"])
	{
		for (auto it = entity.begin(); it != entity.end(); ++it)
		{
			Entity* gameEntity = world->create();
			int oldID = 0;
			for (const auto& component : it.value())
			{
				std::string componentName = component.begin().key();
				if (componentName == "EntityIdentifier")
				{
					gameEntity->Assign<EntityIdentifier>();
					oldID = component["EntityIdentifier"][0]["m_EntityId"];
					gameEntity->get<EntityIdentifier>()->m_ComponentName = component["EntityIdentifier"][0]["m_ComponentName"];
					gameEntity->get<EntityIdentifier>()->m_EntityName = component["EntityIdentifier"][0]["m_EntityName"];
					gameEntity->get<EntityIdentifier>()->m_HasParent = component["EntityIdentifier"][0]["m_HasParent"];
					gameEntity->get<EntityIdentifier>()->m_ParentEntityId = component["EntityIdentifier"][0]["m_ParentEntityId"];
					gameEntity->get<EntityIdentifier>()->m_EntityId = gameEntity->getEntityId();
				}
				else if (componentName == "Transform")
				{
					AssignComponents<Transform>(gameEntity, component["Transform"][0]);
				}
				else if (componentName == "BoxCollider")
				{
					AssignComponents<BoxCollider>(gameEntity, component["BoxCollider"][0]);
				}

				else if (componentName == "Camera")
				{
					AssignComponents<Camera>(gameEntity, component["Camera"][0]);
				}

				else if (componentName == "Light")
				{
					AssignComponents<Light>(gameEntity, component["Light"][0]);
				}

				else if (componentName == "Movement")
				{
					AssignComponents<Movement>(gameEntity, component["Movement"][0]);
				}

				else if (componentName == "StaticMesh")
				{
					std::string fileName = component["StaticMesh"][0]["m_FileName"];
					gameEntity->Assign<StaticMesh>(fileName);
					gameEntity->get<StaticMesh>().get().m_ComponentName = component["StaticMesh"][0]["m_ComponentName"];
					gameEntity->get<StaticMesh>().get().m_FileName = component["StaticMesh"][0]["m_FileName"];
					gameEntity->get<StaticMesh>().get().m_IsModelCreated = component["StaticMesh"][0]["m_IsModelCreated"];
				}
				else if (componentName == "Debug")
				{
					AssignComponents<Debug>(gameEntity, component["Debug"][0]);
				}
				else if (componentName == "Sound")
				{
					AssignComponents<Sound>(gameEntity, component["Sound"][0]);
				}
				else if (componentName == "RigidBody")
				{
					AssignComponents<RigidBody>(gameEntity, component["RigidBody"][0]);
				}
				else if (componentName == "UI")
				{
					AssignComponents<UI>(gameEntity, component["UI"][0]);
				}
				else if (componentName == "Space")
				{
					AssignComponents<Space>(gameEntity, component["Space"][0]);
				}
				else if (componentName == "DynamicText")
				{
					AssignComponents<DynamicText>(gameEntity, component["DynamicText"][0]);
				}
				else if (componentName == "Sprite2D")
				{
					AssignComponents<Sprite2D>(gameEntity, component["Sprite2D"][0]);
				}
				else if (componentName == "PlayerInformation")
				{
					AssignComponents<PlayerInformation>(gameEntity, component["PlayerInformation"][0]);
				}
				else if (componentName == "Interactive")
				{
					AssignComponents<Interactive>(gameEntity, component["Interactive"][0]);
				}
				else if (componentName == "Script")
				{
					if (component["Script"][0]["m_ComponentName"].get<std::string>() == "FreeCameraScript")
					{
						AssignComponents<FreeCameraScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "SampleScript")
					{
						AssignComponents<SampleScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "PlayerScript")
					{
						AssignComponents<PlayerScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "POVCameraScript")
					{
						AssignComponents<POVCameraScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "TestUIScript")
					{
						AssignComponents<TestUIScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "DynamicTextScript")
					{
						AssignComponents<DynamicTextScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "IntroCameraScript")
					{
						AssignComponents<IntroCameraScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "OutroScript")
					{
						AssignComponents<OutroScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "DrawerScript")
					{
						AssignComponents<DrawerScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "IntroDoorScript")
					{
						AssignComponents<IntroDoorScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "DoorScript")
					{
						AssignComponents<DoorScript>(gameEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "IntroButtonScript")
					{
						AssignComponents<IntroButtonScript>(gameEntity, component["Script"][0]);
					}
				}
			}
			m_entityContainer.push_back({ gameEntity, oldID });
		}
	}


	for (const auto& gameEntityChild : m_entityContainer)
	{
		for (const auto& gameEntityParent : m_entityContainer)
		{
			if (gameEntityChild.first->get<EntityIdentifier>().get().m_HasParent == true)
			{
				if (gameEntityChild.first->get<EntityIdentifier>().get().m_ParentEntityId == gameEntityParent.second)
				{
					SetParent(gameEntityChild.first, gameEntityParent.first);
				}
			}
		}
	}

	for (const auto& entity : world->GetEntities())
	{
		if (entity->get<EntityIdentifier>()->m_EntityName == "Main Camera")
		{
			world->destroy(entity);
		}
	}

	m_entityContainer.clear();
	return world;
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

	if (WorldManager::GetInstance()->GetCurrentWorld() == m_IntroWorld)
	{
		for (const auto& introEntity : m_IntroWorld->GetEntities())
		{
			if (introEntity->get<EntityIdentifier>()->m_EntityName == "PlayerCamera")
			{
				if (introEntity->get<Transform>()->m_Position.GetZ() > -555.0f && introEntity->get<Transform>()->m_Position.GetZ() < -550.0f)
				{

					if (m_GameWorld != nullptr)
					{
						WorldManager::GetInstance()->ChangeWorld(m_GameWorld);
					}
				}
			}
		}
	}
	

	if (InputManager::GetInstance()->GetKeyDown(Key::F9))
	{
		WorldManager::GetInstance()->ChangeWorld(m_OutroWorld);
	}

	if ( WorldManager::GetInstance()->GetCurrentWorld() == m_OutroWorld)
	{
		for (auto& entity : m_OutroWorld->GetEntities())
		{
			if (entity->has<Script>())
			{
				if (entity->get<Sprite2D>()->m_Position[0] == 2024)
				{
					m_IntroWorld = DeserializeGame("scene/TitleScene.scene");
					WorldManager::GetInstance()->ChangeWorld(m_IntroWorld);
				}
			}
		}
	}

}

void GameApp::Render()
{
	__super::Render();
}
