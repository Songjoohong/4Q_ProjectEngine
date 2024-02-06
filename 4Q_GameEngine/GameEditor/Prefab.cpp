#include "pch.h"
#include "Prefab.h"
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

using json = nlohmann::json;

PrefabManager::PrefabManager()
{

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

	//Entity* prefabEntity = 

	return nullptr;
}

void PrefabManager::DeleteAllDataInJsonFile(const std::string& filename)
{
	std::string fullPath = basePath + filename;

	json emptyJson;

	std::ofstream outputFile(fullPath);

	outputFile << emptyJson.dump(4);
	outputFile.close();
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

	for (const auto& child : entity->m_children)
	{
		RecursiveSaveComponents(child, prefabData);
	}
}
