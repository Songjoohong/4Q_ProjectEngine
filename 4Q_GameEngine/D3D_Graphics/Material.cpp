#include "pch.h"
#include "Material.h"
#include "D3D_Graphics.h"
#include "ResourceManager.h"

#include <assimp/scene.h>
#include <assimp/types.h>
void MaterialTexture::Create(const std::wstring& filePath)
{
	HRESULT hr = S_OK;
    m_filePath = filePath;
    
    DirectX::TexMetadata metadata1;
    DirectX::ScratchImage scratchImage;
	// Load the Texture
	hr = DirectX::CreateDDSTextureFromFile(Renderer::Instance->m_pDevice.Get(), filePath.c_str(), nullptr, &m_pTextureRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateWICTextureFromFile(Renderer::Instance->m_pDevice.Get(), filePath.c_str(), nullptr, &m_pTextureRV);
		if (FAILED(hr))
		{
            hr = DirectX::LoadFromTGAFile(filePath.c_str(), &metadata1, scratchImage);
            hr = DirectX::CreateShaderResourceView(Renderer::Instance->m_pDevice.Get(), scratchImage.GetImages(), scratchImage.GetImageCount(), metadata1, m_pTextureRV.GetAddressOf());
            if (FAILED(hr))
            {
                MessageBoxW(NULL, GetComErrorString(hr), filePath.c_str(), MB_OK);
            }
		}
	}
}

void Material::Create(aiMaterial* material)
{
    aiString texturePath;
    std::filesystem::path path;
    std::wstring folderPath;
    std::string name;
    std::wstring basePath = L"../Resource/FBXLoad_Test/texture";

    for (const auto& entry : std::filesystem::recursive_directory_iterator(basePath))
    {
        if (entry.is_regular_file() )//&& (entry.path().extension() == L".png" || entry.path().extension() == L".jpg"))
        {
            // fbx 이름이랑 폴더 이름이랑 같으면 folderPath 생성
            folderPath = entry.path().parent_path().wstring();

            std::wstring extractedString;
            size_t lastSlash = folderPath.find_last_of('\\');
            size_t lastDot = folderPath.find_last_of('.');

            if (lastSlash != std::string::npos && lastDot != std::string::npos)
            {
                extractedString = folderPath.substr(lastSlash + 1, lastDot - lastSlash - 1);
            }

            if (m_fileName == extractedString)
            {
                name = material->GetName().C_Str();
                break;
            }
        }
    }

    // 텍스처 로드
    if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        m_pDiffuseRV = ResourceManager::Instance->CreateMaterial(finalPath);
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        m_pNormalRV = ResourceManager::Instance->CreateMaterial(finalPath);
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        m_pSpecularRV = ResourceManager::Instance->CreateMaterial(finalPath);
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        m_pEmissiveRV = ResourceManager::Instance->CreateMaterial(finalPath);
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        m_pOpacityRV = ResourceManager::Instance->CreateMaterial(finalPath);
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_METALNESS, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        m_pMetalicRV = ResourceManager::Instance->CreateMaterial(finalPath);
    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_SHININESS, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        m_pRoughnessRV = ResourceManager::Instance->CreateMaterial(finalPath);

    }

    if (AI_SUCCESS == material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &texturePath))
    {
        path = ToWString(std::string(texturePath.C_Str()));
        std::wstring finalPath = folderPath + L"/" + path.filename().wstring();
        m_pAmbientOcclusionRV = ResourceManager::Instance->CreateMaterial(finalPath);
    }
    if(m_pMetalicRV)
    {
        if (m_pOpacityRV)
            m_pixelShader.SetShader(L"../Resource/PixelShader");
        else
            m_pixelShader.SetShader(L"OpaquePBR");
    }
    else
    {
        if (m_pOpacityRV)
            m_pixelShader.SetShader(L"PS_NoMetalOpacity");
        else
            m_pixelShader.SetShader(L"PS_NoMetalic");
    }
    
    //m_shadowPixelShader.SetShader(L"ShadowPixelShader");
}

void Material::SetFileName(const std::wstring& fileName)
{
	m_fileName = fileName;
}
