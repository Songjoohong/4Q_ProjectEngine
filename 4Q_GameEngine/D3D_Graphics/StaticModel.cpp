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
	if (nullptr == sceneResource)
	{
		return;
	}
	m_meshInstance.resize(m_pStaticSceneResource->m_meshes.size());

	for (UINT i = 0; i < m_pStaticSceneResource->m_meshes.size(); i++)
	{
		StaticMeshResource* meshResource = &m_pStaticSceneResource->m_meshes[i];
		Material* material = m_pStaticSceneResource->GetMeshMaterial(i);
		m_meshInstance[i].Create(meshResource, &m_worldTransform, material);
		m_boundingBox.Center = m_worldTransform.Translation() +Vector3(0, sceneResource->m_AABBmax.y - sceneResource->m_AABBmin.y, 0) * 0.5;
	}

}

bool StaticModel::Load(string filename)
{
	shared_ptr<StaticSceneResource> sceneResource = ResourceManager::Instance->CreateStaticMeshResource(Renderer::Instance->GetPath()  + filename);
	if(!sceneResource)
		return false;
	m_boundingBox.Center = Math::Vector3(sceneResource->m_AABBmin + sceneResource->m_AABBmax) * 0.5f;	// Calculate extent	
	m_boundingBox.Extents = Math::Vector3(sceneResource->m_AABBmax - sceneResource->m_AABBmin) * 0.5f;
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


