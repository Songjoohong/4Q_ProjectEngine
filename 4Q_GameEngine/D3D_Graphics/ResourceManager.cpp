#include "pch.h"
#include "ResourceManager.h"
#include "StaticMeshResource.h"
#include "Material.h"

ResourceManager* ResourceManager::Instance = nullptr;

ResourceManager::ResourceManager()
{
    assert(Instance == nullptr);
    Instance = this;
}

ResourceManager::~ResourceManager()
{
}

std::shared_ptr<StaticSceneResource> ResourceManager::CreateStaticMeshResource(std::string filePath)
{
    std::string key = filePath;
    auto it = m_staticMeshMap.find(key);
    if (it != m_staticMeshMap.end())
    {
        std::shared_ptr<StaticSceneResource> resourcePtr = it->second.lock();
        if (resourcePtr)
        {
            return resourcePtr;
        }
        else
        {
            m_staticMeshMap.erase(it);
        }
    }

    std::filesystem::path path = ToWString(filePath);
    if (!std::filesystem::exists(path))
    {
        LOG_MESSAGEA("Error file not Found : %s", filePath.c_str());
        return nullptr;
    }

    std::shared_ptr<StaticSceneResource> resourcePtr = std::make_shared<StaticSceneResource>();
    resourcePtr->Create(filePath.c_str());
    m_staticMeshMap[filePath] = resourcePtr;
    LOG_MESSAGEA("Complete file : %s", filePath.c_str());
    return resourcePtr;
}

std::shared_ptr<MaterialTexture> ResourceManager::CreateMaterial(std::wstring filePath)
{
    auto it = m_materialMap.find(filePath);
    if (it != m_materialMap.end())
    {
        std::shared_ptr<MaterialTexture> resourcePtr = it->second.lock();
        if (resourcePtr)
        {
            return resourcePtr;
        }
        else
        {
            m_materialMap.erase(it);
        }
    }

    std::filesystem::path path = filePath;
    if (!std::filesystem::exists(path))
    {
        LOG_MESSAGEA("Error file not Found : %s", filePath.c_str());
        return nullptr;
    }

    std::shared_ptr<MaterialTexture> resourcePtr = std::make_shared<MaterialTexture>();
    resourcePtr->Create(filePath);
    m_materialMap[filePath] = resourcePtr;
    LOG_MESSAGEA("Complete file : %s", filePath.c_str());
    return resourcePtr;
}
