#pragma once

#include "StaticMeshInstance.h"


class MaterialTexture;
class StaticSceneResource;

class Environment
{
public:
	Math::Matrix m_worldTransform;

	std::shared_ptr<StaticSceneResource> m_pSceneResource;
	std::shared_ptr<MaterialTexture>	 m_pEnvironmentTextureResource;
	std::shared_ptr<MaterialTexture>	 m_pIBLDiffuseTextureResource;
	std::shared_ptr<MaterialTexture>	 m_pIBLSpecularTextureResource;
	std::shared_ptr<MaterialTexture>	 m_pIBLBRDFTextureResource;
	StaticMeshInstance					 m_meshInstance;


public:
	bool ReadEnvironmentMeshFromFBX(std::string filename);
	bool ReadEnvironmentTextureFromDDS(std::wstring filename);
	bool ReadIBLDiffuseTextureFromDDS(std::wstring filename);
	bool ReadIBLSpecularTextureFromDDS(std::wstring filename);
	bool ReadIBLBRDFTextureFromDDS(std::wstring filename);
};

