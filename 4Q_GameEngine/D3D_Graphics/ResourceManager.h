#pragma once

class StaticSceneResource;
class MaterialTexture;
class Model;

class ResourceManager
{
public:
	static ResourceManager* Instance;

	ResourceManager();
	~ResourceManager();

    void CreateModel(string fileName);

    std::map<std::string, shared_ptr<Model>> m_pOriginalModels;

public:
    std::map<std::string, std::weak_ptr<StaticSceneResource>> m_staticMeshMap;
    //std::map<std::string, std::weak_ptr<SkeletalMeshResource>> m_skeletalMeshMap;
    std::map<std::wstring, std::weak_ptr<MaterialTexture>> m_materialMap;
    //std::map<std::string, std::weak_ptr<Animation>> m_animationMap;

    std::shared_ptr<StaticSceneResource> CreateStaticMeshResource(std::string filePath);
    //std::shared_ptr<SkeletalMeshResource> CreateSkeletalMeshResource(std::string filePath);
    std::shared_ptr<MaterialTexture> CreateMaterial(std::wstring filePath);
    //std::shared_ptr<Animation> CreateAnimation(std::string filePath);
	
};

