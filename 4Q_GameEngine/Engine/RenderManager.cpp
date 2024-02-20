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

void RenderManager::GameAppRender()
{
	Renderer::Instance->GameAppRender();
}

void RenderManager::EditorRender()
{
	Renderer::Instance->EditorRender();
}


void RenderManager::RenderEnd()
{
	Renderer::Instance->RenderEnd();
}


void RenderManager::AddStaticMesh(const std::string& fileName, Math::Matrix worldTM) const
{
	Renderer::Instance->AddStaticModel(fileName, worldTM);
}


void RenderManager::AddColliderBox(const Vector3D center, const Vector3D extents, const bool collision, DirectX::SimpleMath::Matrix worldTM)
{
	Renderer::Instance->AddColliderBox(Vector3(center.GetX(), center.GetY(), center.GetZ()), Vector3(extents.GetX(), extents.GetY(), extents.GetZ()),  worldTM);
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

void RenderManager::AddText(int entID, const std::string& text, const Vector3D& pos)
{
	Vector3D p = { pos.m_X, pos.m_Y + 50.f,pos.m_Y };
	Renderer::Instance->AddTextInformation(entID, text, p);
}

void RenderManager::AddSprite(int entID, const std::string& filePath, POINT pos, float layer)
{
	Renderer::Instance->AddSpriteInformation(entID, filePath, DirectX::XMFLOAT2{static_cast<float>(pos.x), static_cast<float>(pos.y)}, layer);
}

void RenderManager::AddDynamicText(int entID, const vector<std::wstring>& textVector)
{
	Renderer::Instance->AddDynamicTextInformation(entID, textVector);
}

void RenderManager::EditText(int entID, const std::string& text, const Vector3D& pos)
{
	Vector3D p = { pos.m_X, pos.m_Y + 50.f,pos.m_Y };
	Renderer::Instance->EditTextInformation(entID, text, p);
}

void RenderManager::EditSprite(int entID, bool isRendered)
{
	Renderer::Instance->EditSpriteInformation(entID, isRendered);
}

void RenderManager::EditDynamicText(int size, int index, bool enable)
{
	Renderer::Instance->EditDynamicTextInformation(size, index, enable);
}

void RenderManager::DeleteText(int entID)
{
	Renderer::Instance->DeleteTextInformation(entID);
}

void RenderManager::DeleteSprite(int entID)
{
	Renderer::Instance->DeleteSpriteInformation(entID);
}

void RenderManager::DeleteDynamicText(int entID)
{
	Renderer::Instance->DeleteDynamicTextInformation(entID);
}




