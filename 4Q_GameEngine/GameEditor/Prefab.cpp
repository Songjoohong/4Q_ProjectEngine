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

	std::ofstream outputFile(fullPath, std::ios::app);

	SaveComponents<EntityIdentifier>(_selectedEntity, prefabData);
	SaveComponents<Transform>(_selectedEntity, prefabData);
	SaveComponents<StaticMesh>(_selectedEntity, prefabData);
	SaveComponents<BoxCollider>(_selectedEntity, prefabData);
	SaveComponents<Camera>(_selectedEntity, prefabData);
	SaveComponents<Light>(_selectedEntity, prefabData);
	SaveComponents<Movement>(_selectedEntity, prefabData);
	SaveComponents<Debug>(_selectedEntity, prefabData);
	SaveComponents<Sound>(_selectedEntity, prefabData);
	SaveComponents<Sprite2D>(_selectedEntity, prefabData);

	outputFile << std::setw(4) << prefabData << std::endl;

	outputFile.close();
}

void PrefabManager::LoadPrefab(const std::string& _filename)
{
	std::string fullPath = basePath + _filename;

	std::ifstream inputFile(fullPath);
	json prefabData;
	inputFile >> prefabData;
	inputFile.close();

	Entity* prefabEntity;

}

void PrefabManager::DeleteAllDataInJsonFile(const std::string& filename)
{
	std::string fullPath = basePath + filename;

	json emptyJson;

	std::ofstream outputFile(fullPath);

	outputFile << emptyJson.dump(4);
	outputFile.close();
}
