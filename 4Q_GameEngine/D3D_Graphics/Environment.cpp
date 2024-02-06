#include "pch.h"
#include "Environment.h"
#include "ResourceManager.h"
#include "D3D_Graphics.h"
#include "StaticMeshResource.h"

bool Environment::ReadEnvironmentMeshFromFBX(std::string filePath)
{
    m_pSceneResource = ResourceManager::Instance->CreateEnvironmentMeshResource(filePath);
    if(nullptr == m_pSceneResource)
        return false;
    m_worldTransform = Math::Matrix::CreateScale(10000, 10000, 10000);
    m_meshInstance.Create(&m_pSceneResource->m_meshes[0], &m_worldTransform, nullptr);
    return true;
}

bool Environment::ReadEnvironmentTextureFromDDS(std::wstring filename)
{
    wstring filepath = filename + L"EnvHDR.dds";
    m_pEnvironmentTextureResource = ResourceManager::Instance->CreateMaterial(filepath);
    if (m_pEnvironmentTextureResource == nullptr)
        return false;

    return true;
}

bool Environment::ReadIBLDiffuseTextureFromDDS(std::wstring filename)
{
    wstring filepath = filename + L"DiffuseHDR.dds";
    m_pIBLDiffuseTextureResource = ResourceManager::Instance->CreateMaterial(filepath);
    if (m_pIBLDiffuseTextureResource == nullptr)
        return false;
    return true;
}

bool Environment::ReadIBLSpecularTextureFromDDS(std::wstring filename)
{
    wstring filepath = filename + L"SpecularHDR.dds";
    m_pIBLSpecularTextureResource = ResourceManager::Instance->CreateMaterial(filepath);
    if (m_pIBLSpecularTextureResource == nullptr)
        return false;
    return true;
}

bool Environment::ReadIBLBRDFTextureFromDDS(std::wstring filename)
{
    wstring filepath = filename + L"BRDF.dds";
    m_pIBLBRDFTextureResource = ResourceManager::Instance->CreateMaterial(filepath);
    if (m_pIBLBRDFTextureResource == nullptr)
        return false;
    return true;
}
