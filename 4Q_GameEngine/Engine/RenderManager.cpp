#include "pch.h"
#include "RenderManager.h"

#include "../D3D_Graphics/D3D_Graphics.h"
#include "../D3D_Graphics/ResourceManager.h"

void RenderManager::Initialize(HWND* hwnd, UINT width, UINT height)
{
	m_Renderer = new Renderer();
	m_ResourceManager = new ResourceManager();
	m_Renderer->Instance->Initialize(hwnd,width,height);
}

void RenderManager::Render()
{
	m_Renderer->Instance->Render();
}

void RenderManager::AddStaticMesh(std::string filename, Vector3D pos, Vector3D rot)
{
	m_Renderer->Instance->AddStaticModel(filename);
}


