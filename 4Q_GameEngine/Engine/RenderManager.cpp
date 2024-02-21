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

bool RenderManager::Culling(DirectX::BoundingBox boundingBox)
{
	DirectX::BoundingFrustum m_frustumCmaera;
	DirectX::BoundingFrustum::CreateFromMatrix(m_frustumCmaera, Renderer::Instance->GetProjectionMatrix());
	m_frustumCmaera.Transform(m_frustumCmaera, Renderer::Instance->GetViewMatrix().Invert());
	return m_frustumCmaera.Intersects(boundingBox);
}


void RenderManager::AddStaticMesh(const std::string& fileName, Math::Matrix worldTM) const
{
	Renderer::Instance->AddStaticModel(fileName, worldTM);
}

void RenderManager::AddOutlineModel(const std::string& fileName, DirectX::SimpleMath::Matrix worldTM) const
{
	Renderer::Instance->AddOutlineModel(fileName, worldTM);
}

void RenderManager::AddColliderBox(const Vector3D center, const Vector3D extents, const Vector3D rotation)
{
	Renderer::Instance->AddColliderBox(Vector3(center.GetX(), center.GetY(), center.GetZ()), Vector3(extents.GetX(), extents.GetY(), extents.GetZ()), Vector3(rotation.GetX(), rotation.GetY(), rotation.GetZ()));
}

void RenderManager::AddBoundingBox(DirectX::BoundingBox boundingBox)
{
	Renderer::Instance->AddBoundingBox(boundingBox);
}


void RenderManager::CreateModel(string filename, DirectX::BoundingBox& boundingBox)
{
	Renderer::Instance->CreateModel(filename,boundingBox);
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

void RenderManager::AddSprite(World* world, int entID, const std::string& filePath, POINT pos, float layer)
{
	Renderer::Instance->AddSpriteInformation(world, entID, filePath, DirectX::XMFLOAT2{static_cast<float>(pos.x), static_cast<float>(pos.y)}, layer);
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

void RenderManager::EditSprite(int entID, Sprite2D& sprite2D)
{
	Renderer::Instance->EditSpriteInformation(entID, sprite2D);
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




