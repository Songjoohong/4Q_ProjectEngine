#include "pch.h"
#include "Prefab.h"

// Components Headers
#include "../Engine/Transform.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/Camera.h"
#include "../Engine/Light.h"
#include "../Engine/EntityIdentifier.h"
#include "../Engine/Movement.h"
#include "../Engine/Script.h"
#include "../Engine/SampleScript.h"
#include "../Engine/StaticMesh.h"
#include "../Engine/Debug.h"
#include "../Engine/Sound.h"
#include "../Engine/Sprite2D.h"
#include "../Engine/CameraScript.h"
#include "NameManager.h"
#include <set>
using json = nlohmann::json;

PrefabManager::PrefabManager(ECS::World* currentWorld, std::shared_ptr<NameManager> nameManager)
{
	m_CurrentWorld = currentWorld;
	m_NameManager = nameManager;
}

PrefabManager::~PrefabManager()
{
}

void PrefabManager::SavePrefab(ECS::Entity* _selectedEntity, const std::string& _filename)
{
	std::string fullPath = basePath + _filename;

	json prefabData;

	if (!_selectedEntity->m_children.empty())
	{
		RecursiveSaveComponents(_selectedEntity, prefabData);

		std::ofstream outputFile(fullPath);
		outputFile << std::setw(4) << prefabData << std::endl;

		outputFile.close();
	}
	else
	{
		RecursiveSaveComponents(_selectedEntity, prefabData);
		std::ofstream outputFile(fullPath);
		outputFile << std::setw(4) << prefabData << std::endl;

		outputFile.close();
	}
}


ECS::Entity* PrefabManager::LoadPrefab(const std::string& _filename)
{
	std::string fullPath = basePath + _filename;

	std::ifstream inputFile(fullPath);
	json prefabData;
	inputFile >> prefabData;
	inputFile.close();

	for (const auto& entity : prefabData["Prefabs"])
	{
		for (auto it = entity.begin(); it != entity.end(); ++it)
		{
			Entity* prefabEntity = m_CurrentWorld->create();
			int oldID = 0;
			for (const auto& component : it.value())
			{
				std::string componentName = component.begin().key();
				if (componentName == "EntityIdentifier")
				{
					prefabEntity->Assign<EntityIdentifier>();
					oldID = component["EntityIdentifier"][0]["m_EntityId"];
					prefabEntity->get<EntityIdentifier>()->m_ComponentName = component["EntityIdentifier"][0]["m_ComponentName"];
					prefabEntity->get<EntityIdentifier>()->m_EntityName = component["EntityIdentifier"][0]["m_EntityName"];
					prefabEntity->get<EntityIdentifier>()->m_HasParent = component["EntityIdentifier"][0]["m_HasParent"];
					prefabEntity->get<EntityIdentifier>()->m_ParentEntityId = component["EntityIdentifier"][0]["m_ParentEntityId"];
					prefabEntity->get<EntityIdentifier>()->m_EntityId = prefabEntity->getEntityId();
				}
				else if (componentName == "Transform")
				{
					AssignComponents<Transform>(prefabEntity, component["Transform"][0]);
				}
				else if (componentName == "BoxCollider")
				{
					AssignComponents<BoxCollider>(prefabEntity, component["BoxCollider"][0]);
				}

				else if (componentName == "Camera")
				{
					AssignComponents<Camera>(prefabEntity, component["Camera"][0]);
				}

				else if (componentName == "Light")
				{
					AssignComponents<Light>(prefabEntity, component["Light"][0]);
				}

				else if (componentName == "Movement")
				{
					AssignComponents<Movement>(prefabEntity, component["Movement"][0]);
				}

				else if (componentName == "StaticMesh")
				{
					std::string fileName = component["StaticMesh"][0]["m_FileName"];
					prefabEntity->Assign<StaticMesh>(fileName);
					prefabEntity->get<StaticMesh>().get().m_ComponentName = component["StaticMesh"][0]["m_ComponentName"];
					prefabEntity->get<StaticMesh>().get().m_FileName = component["StaticMesh"][0]["m_FileName"];
					prefabEntity->get<StaticMesh>().get().m_IsModelCreated = component["StaticMesh"][0]["m_IsModelCreated"];
				}
				else if (componentName == "Debug")
				{
					AssignComponents<Debug>(prefabEntity, component["Debug"][0]);
				}
				else if (componentName == "Sound")
				{
					AssignComponents<Sound>(prefabEntity, component["Sound"][0]);
				}
				else if (componentName == "CameraScript")
				{
					AssignComponents<CameraScript>(prefabEntity, component["CameraScript"][0]);
				}
			}
			m_prefabContainer.push_back({ prefabEntity, oldID });
		}
	}


	for (const auto& prefabChild : m_prefabContainer)
	{
		for (const auto& prefabParent : m_prefabContainer)
		{
			if (prefabChild.first->get<EntityIdentifier>().get().m_HasParent == true)
			{
				if (prefabChild.first->get<EntityIdentifier>().get().m_ParentEntityId == prefabParent.second)
				{
					SetParent(prefabChild.first, prefabParent.first);
				}
			}
		}
	}

	for (const auto& prefab : m_prefabContainer)
	{
		m_NameManager->AddEntityName(prefab.first);
	}

	return m_prefabContainer[0].first;
}

void PrefabManager::SetParent(ECS::Entity* child, ECS::Entity* parent)
{
	child->get<EntityIdentifier>().get().m_ParentEntityId = parent->getEntityId();
	child->get<EntityIdentifier>().get().m_HasParent = true;

	parent->addChild(child);
}

void PrefabManager::DeleteAllDataInJsonFile(const std::string& filename)
{
	std::string fullPath = basePath + filename;

	json emptyJson;

	std::ofstream outputFile(fullPath);

	outputFile << emptyJson.dump(4);
	outputFile.close();
}

void PrefabManager::SetContext(ECS::World* world)
{
	m_CurrentWorld = world;
}

ECS::Entity* PrefabManager::FindEntityByName(std::string entityName)
{
	for (const auto& entity : m_CurrentWorld->GetEntities())
	{
		if (entity->get<EntityIdentifier>().get().m_EntityName == entityName)
			return entity;
	}	
}

void PrefabManager::RecursiveSaveComponents(ECS::Entity* entity, json& prefabData)
{
	SaveComponents<EntityIdentifier>(entity, prefabData);
	SaveComponents<Transform>(entity, prefabData);
	SaveComponents<StaticMesh>(entity, prefabData);
	SaveComponents<BoxCollider>(entity, prefabData);
	SaveComponents<Camera>(entity, prefabData);
	SaveComponents<Light>(entity, prefabData);
	SaveComponents<Movement>(entity, prefabData);
	SaveComponents<Debug>(entity, prefabData);
	SaveComponents<Sound>(entity, prefabData);
	SaveComponents<Sprite2D>(entity, prefabData);
	SaveComponents<CameraScript>(entity, prefabData);

	for (const auto& child : entity->m_children)
	{
		RecursiveSaveComponents(child, prefabData);
	}
}
