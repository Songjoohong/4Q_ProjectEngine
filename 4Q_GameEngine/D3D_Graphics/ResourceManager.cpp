#include "pch.h"
#include "ResourceManager.h"
#include "StaticMeshResource.h"
#include "Material.h"
#include "Model.h"
#include "StaticModel.h"
#include "Environment.h"

ResourceManager* ResourceManager::Instance = nullptr;

ResourceManager::ResourceManager()
{
    assert(Instance == nullptr);
    Instance = this;
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::CreateModel(string fileName,DirectX::BoundingBox& boundingBox)
{
    auto it = m_pOriginalModels.find(fileName);
    if (it != m_pOriginalModels.end())
    {
        if (nullptr != it->second)
        {
            return;
        }
        else
        {
            m_pOriginalModels.erase(it);
        }
    }
    shared_ptr<Model> pModel = make_shared<StaticModel>();
    dynamic_cast<StaticModel*>(pModel.get())->Load(fileName);
    m_pOriginalModels[fileName] = pModel;
    boundingBox = dynamic_cast<StaticModel*>(pModel.get())->m_boundingBox;
}

void ResourceManager::CreateEnvironment(string filename)
{
    auto it = m_pOriginalEnvironments.find(filename);
    if (it != m_pOriginalEnvironments.end())
    {
        if (nullptr != it->second)
            return;
        else
        {
            m_pOriginalEnvironments.erase(it);
        }
    }
    shared_ptr<Environment> pEnvironment = make_shared<Environment>();
    pEnvironment->ReadEnvironmentMeshFromFBX(Renderer::Instance->GetPath() + "cubeMap.fbx");
    pEnvironment->ReadEnvironmentTextureFromDDS(ToWString(Renderer::Instance->GetPath() + filename));
    pEnvironment->ReadIBLDiffuseTextureFromDDS(ToWString(Renderer::Instance->GetPath() + filename));
    pEnvironment->ReadIBLSpecularTextureFromDDS(ToWString(Renderer::Instance->GetPath() + filename));
    pEnvironment->ReadIBLBRDFTextureFromDDS(ToWString(Renderer::Instance->GetPath() + filename));

    m_pOriginalEnvironments[filename] = pEnvironment;
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

std::shared_ptr<StaticSceneResource> ResourceManager::CreateEnvironmentMeshResource(std::string filePath)
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
    resourcePtr->CreateEnvironment(filePath.c_str());
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
