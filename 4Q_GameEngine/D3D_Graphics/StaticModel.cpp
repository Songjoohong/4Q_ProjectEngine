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
		m_meshInstance[i].Create(meshResource, m_worldTransform, material);
	}
	/*Math::Vector4 aabbMin = Math::Vector4(sceneResource->m_AABBmin.x, sceneResource->m_AABBmin.y, sceneResource->m_AABBmin.z, 1.0f);
	Math::Vector4 aabbMax = Math::Vector4(sceneResource->m_AABBmax.x, sceneResource->m_AABBmax.y, sceneResource->m_AABBmax.z, 1.0f);
	
	

	aabbMin = DirectX::XMVector4Transform(aabbMin, m_worldTransform);
	aabbMax = DirectX::XMVector4Transform(aabbMax, m_worldTransform);*/
	Math::Vector3 aabbMin = m_pStaticSceneResource->m_BoundingBoxMin;
	Math::Vector3 aabbMax = m_pStaticSceneResource->m_BoundingBoxMax;

	Math::Vector3 scale, position;
	Math::Quaternion rotation;
	m_worldTransform.Decompose(scale, rotation, position);

	Vector3 aabb = (aabbMax - aabbMin) / 2;
	m_boundingBox.Extents = Vector3(aabb.x, aabb.y, aabb.z)*scale;
	m_boundingBox.Center = m_worldTransform.Translation()+Vector3(0, sceneResource->m_BoundingBoxMax.y - sceneResource->m_BoundingBoxMin.y, 0) * 0.5;
	//
	// m_boundingBox.Center=Vector3(DirectX::XMVector3TransformCoord(m_worldTransform.Translation(), m_worldTransform));

}

bool StaticModel::Load(string filename)
{
	shared_ptr<StaticSceneResource> sceneResource = ResourceManager::Instance->CreateStaticMeshResource(Renderer::Instance->GetPath()  + filename);
	if(!sceneResource)
		return false;
	m_boundingBox.Center = Math::Vector3(sceneResource->m_BoundingBoxMin + sceneResource->m_BoundingBoxMax) * 0.5f;	// Calculate extent	
	SetSceneResource(sceneResource);
	//m_boundingBox.Extents = Math::Vector3(sceneResource->m_AABBmax - sceneResource->m_AABBmin) * 0.5f;
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


