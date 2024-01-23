#include "pch.h"

#include "../D3D_Graphics/D3D_Graphics.h"
#include "../D3D_Graphics/ResourceManager.h"

#include "RenderManager.h"

void RenderManager::Initialize(HWND* hwnd, UINT width, UINT height)
{
	m_Renderer = new Renderer();
	m_ResourceManager = new ResourceManager();
	m_Renderer->Instance->Initialize(hwnd,width,height);
}

void RenderManager::AddStaticModel(std::string filename, Vector3D pos, Vector3D view)
{
	m_Renderer->Instance->AddStaticModel(filename);
}

void RenderManager::Render()
{
	m_Renderer->Instance->Render();
}
