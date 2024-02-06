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


void RenderManager::UnInitialize()
{
	m_Renderer->Instance->UnInitialize();
}
void RenderManager::Update()
{
	Renderer::Instance->Update();

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


void RenderManager::AddStaticMesh(const std::string& fileName, Math::Matrix worldTM) const
{
	Renderer::Instance->AddStaticModel(fileName, worldTM);
}


void RenderManager::CreateModel(string filename)
{
	Renderer::Instance->CreateModel(filename);
}

void RenderManager::SetBasePath(std::string filePath)
{
	Renderer::Instance->SetPath(filePath);
}


void RenderManager::SetCamera(const DirectX::SimpleMath::Matrix matrix)
{
	Renderer::Instance->SetCamera(matrix);
}

void RenderManager::AddDebug(int entID, const std::string& text, const Vector3D& pos)
{
	Renderer::Instance->AddDebugInformation(entID, text, pos);
}

void RenderManager::AddSprite(int entID, const std::string& filePath, MyPoint pos, float layer)
{
	Renderer::Instance->AddSpriteInformation(entID, filePath, DirectX::XMFLOAT2{static_cast<float>(pos.x), static_cast<float>(pos.y)}, layer);
}

void RenderManager::EditDebug(int entID, const std::string& text, const Vector3D& pos)
{
	Renderer::Instance->EditDebugInformation(entID, text, pos);
}

void RenderManager::EditSprite(int entID, bool isRendered)
{
	Renderer::Instance->EditSpriteInformation(entID, isRendered);
}

void RenderManager::DeleteDebug(int entID)
{
	Renderer::Instance->DeleteDebugInformation(entID);
}

void RenderManager::DeleteSprite(int entID)
{
	Renderer::Instance->DeleteSpriteInformation(entID);
}




