#include "pch.h"
#include "StaticModel.h"
#include "ResourceManager.h"
#include "D3D_Graphics.h"

StaticModel::~StaticModel()
{
	 int a = 0; 
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

void StaticModel::RenderInit()
{
	m_meshInstance[0].Initialize();
}

void StaticModel::Render()
{
	Material* pPrevMaterial = nullptr;
	for (auto it : m_meshInstance)
	{
		if (pPrevMaterial != it.m_pMaterial)
		{
			Renderer::Instance->m_pDeviceContext->PSSetShader(it.m_pMaterial->m_pixelShader.m_pPixelShader.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetSamplers(0, 1, Renderer::Instance->m_pSampler.GetAddressOf());

			Renderer::Instance->ApplyMaterial(it.m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = it.m_pMaterial;
		}

		it.m_worldTransfromCB.worldTransform = m_worldTransform.Transpose();


		it.Render(Renderer::Instance->m_pDeviceContext.Get());
	}
}
