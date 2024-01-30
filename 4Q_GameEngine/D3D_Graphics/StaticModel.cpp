#include "pch.h"
#include "StaticModel.h"
#include "ResourceManager.h"
#include "D3D_Graphics.h"

StaticModel::~StaticModel()
{
}

void StaticModel::SetSceneResource(shared_ptr<StaticSceneResource> sceneResource)
{
	m_pStaticSceneResource = sceneResource;
	m_meshInstance.resize(m_pStaticSceneResource->m_meshes.size());

	for (UINT i = 0; i < m_pStaticSceneResource->m_meshes.size(); i++)
	{
		StaticMeshResource* meshResource = &m_pStaticSceneResource->m_meshes[i];
		Material* material = m_pStaticSceneResource->GetMeshMaterial(i);
		m_meshInstance[i].Create(meshResource, &m_worldTransform, material);
	}

}

bool StaticModel::Load(string filename)
{
	shared_ptr<StaticSceneResource> sceneResource = ResourceManager::Instance->CreateStaticMeshResource(Renderer::Instance->GetPath()  + filename);
	if(!sceneResource)
		return false;
	SetSceneResource(sceneResource);
	RenderInit();
	return true;
}

void StaticModel::MeshInstancePushBack()
{
	for (auto mesh : m_meshInstance)
	{
		Renderer::Instance->m_pMeshInstance.push_back(&mesh);
	}
}

void StaticModel::RenderInit()
{
	m_meshInstance[0].Initialize();
}


