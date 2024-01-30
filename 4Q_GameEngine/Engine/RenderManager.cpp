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

void RenderManager::RenderBegin()
{
	Renderer::Instance->RenderBegin();
}

void RenderManager::Render()
{
	Renderer::Instance->Render();
}

void RenderManager::RenderEnd()
{
	Renderer::Instance->RenderEnd();
}


void RenderManager::AddStaticMesh(const std::string& fileName, Vector3D& pos, Vector3D& rot,Vector3D& scale) const
{
	Vector3 p = Vector3(pos.GetX(), pos.GetY(), pos.GetZ());
	Vector3 r = Vector3(rot.GetX(), rot.GetY(), rot.GetZ());
	Vector3 s = Vector3(scale.GetX(), scale.GetY(), scale.GetZ());

	Renderer::Instance->AddStaticModel(fileName,p,r,s);
}

void RenderManager::CreateModel(string filename)
{
	Renderer::Instance->CreateModel(filename);
}

void RenderManager::SetBasePath(std::string filePath)
{
	Renderer::Instance->SetPath(filePath);
}

void RenderManager::SetCameraPos(Vector3D pos, Vector3D rot)
{
}

void RenderManager::SetCameraPos(Vector3D pos, Vector3D eye, Vector3D up)
{
	Renderer::Instance->SetCamera(Vector3(pos.GetX(), pos.GetY(), pos.GetZ()), Vector3(eye.GetX(), eye.GetY(), eye.GetZ()), Vector3(up.GetX(), up.GetY(), up.GetZ()));
}


