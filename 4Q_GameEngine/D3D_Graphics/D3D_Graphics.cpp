#include "pch.h"
#include "D3D_Graphics.h"
#include "StaticMeshResource.h"
#include "ResourceManager.h"
#include "StaticModel.h"
#include "Environment.h"

#include "DebugDraw.h"

#include "../Engine/Debug.h"

#include "../Engine/TimeManager.h"
#include "../Engine/Sprite2D.h"

#include "RenderTextureClass.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "../Engine/InputManager.h"
#include "../Engine/WorldManager.h"

#define SHADOWMAP_SIZE 2048


Renderer* Renderer::Instance = nullptr;

Renderer::Renderer()
{
	if (nullptr == Instance)
		Instance = this;
}

Renderer::~Renderer()
{
	//UnInitImgui();
}

void Renderer::Clear(float r, float g, float b)
{
	const float clearColor[4] = { r,g,b,1 };
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	//m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
}

void Renderer::Clear(Math::Vector3 color)
{
	const float clearColor[4] = { color.x,color.y,color.z,1 };
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	//m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
}

void Renderer::AddStaticModel(std::string filename, const Math::Matrix& worldTM)
{
	for (auto& model : m_pStaticModels)
	{
		if (nullptr == model->GetSceneResource())
		{
			model->m_worldTransform = worldTM;

			model->Load(filename);
			break;
		}
	}
}

void Renderer::AddColliderBox(Vector3 center, Vector3 extents, Vector3 rotation)
{
	Quaternion rot = Math::Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);
	m_colliderBox.push_back(ColliderBox(center, extents, rot));
}
void Renderer::AddBoundingBox(DirectX::BoundingBox boundingBox)
{
	m_boundingBox.push_back(boundingBox);
}

void Renderer::AddMeshInstance(StaticModel* model)
{
	for (auto& mesh : model->m_meshInstance)
	{
		if (mesh.m_pMaterial->m_pOpacityRV)
			m_pOpacityInstance.push_back(&mesh);
		else
			m_pMeshInstance.push_back(&mesh);
	}
}

void Renderer::AddOutlineMesh(StaticModel* model)
{
	for (auto& mesh : model->m_meshInstance)
	{
		m_pOutlineMesh.push_back(&mesh);
	}
}

void Renderer::AddTextInformation(const int id, const std::string& text, const Vector3D& position)

{

	const DirectX::XMFLOAT3 conversion = ConvertToNDC(position);
	const DirectX::XMFLOAT2 pos = { conversion.x, conversion.y };
	const float depth = conversion.z;

	const TextInformation newText = { id, text, pos, depth };
	m_texts.push_back(newText);
}

void Renderer::AddSpriteInformation(ECS::World* world, int id, const std::string& filePath, const XMFLOAT2 position, float layer)
{
	ComPtr<ID3D11ShaderResourceView> texture;
	const wchar_t* filePathT = ConvertToWchar(filePath);
	HR_T(CreateTextureFromFile(m_pDevice.Get(), filePathT, &texture));
	m_sprites.push_back(SpriteInformation{world, id, layer, true, position, texture });
}

void Renderer::AddDynamicTextInformation(int entId, const vector<std::wstring>& vector)
{
	m_dynamicTexts.push_back({ entId, 0, false, vector });
}

void Renderer::EditTextInformation(int id, const std::string& text, const Vector3D& position)
{
	const DirectX::XMFLOAT3 conversion = ConvertToNDC(position);
	const DirectX::XMFLOAT2 pos = { conversion.x, conversion.y };
	const float depth = conversion.z;

	const auto it = std::find_if(m_texts.begin(), m_texts.end(), [id](const TextInformation& debug)
		{
			return id == debug.mEntityID;
		});
	it->mPosition = pos;
	it->mDepth = depth;
	it->mText = text;
}

void Renderer::EditSpriteInformation(int id, Sprite2D& sprite2D)
{
	const auto it = std::find_if(m_sprites.begin(), m_sprites.end(), [id](const SpriteInformation& sprite)
		{
			return id == sprite.mEntityID;
		});
	it->IsRendered = sprite2D.m_IsRendered;
	it->mLayer = sprite2D.m_Layer;
	it->mPosition = XMFLOAT2{ (float)sprite2D.m_Position[0], (float)sprite2D.m_Position[1] } ;
}

void Renderer::EditDynamicTextInformation(int id, int index, bool enable)
{
	const auto it = std::find_if(m_dynamicTexts.begin(), m_dynamicTexts.end(), [id](const DynamicTextInformation& dynamicText)
		{
			return id == dynamicText.mEntityID;
		});
	it->mIndex = index;
	it->mEnable = enable;
}

void Renderer::DeleteTextInformation(int id)
{
	m_texts.erase(std::find_if(m_texts.begin(), m_texts.end(), [id](const TextInformation& text)
		{
			return id == text.mEntityID;
		}));
}

void Renderer::DeleteSpriteInformation(int id)
{
	m_sprites.erase(std::find_if(m_sprites.begin(), m_sprites.end(), [id](const SpriteInformation& sprite)
		{
			return id == sprite.mEntityID;
		}));
}

void Renderer::DeleteDynamicTextInformation(int id)
{
	m_dynamicTexts.erase(std::find_if(m_dynamicTexts.begin(), m_dynamicTexts.end(), [id](const DynamicTextInformation& dynamicText)
		{
			return id == dynamicText.mEntityID;
		}));
}

void Renderer::CreateModel(string filename, DirectX::BoundingBox& boundingBox)
{
	ResourceManager::Instance->CreateModel(filename, boundingBox);
	StaticModel* pModel = new StaticModel();
	m_pStaticModels.push_back(pModel);
}

void Renderer::CreateViewport(UINT width, UINT height)
{
	//뷰포트 설정
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = (float)width;
	m_viewport.Height = (float)height;
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;

	//Shadow 뷰포트 설정
	m_shadowViewport.TopLeftX = 0;
	m_shadowViewport.TopLeftY = 0;
	m_shadowViewport.Width = (float)SHADOWMAP_SIZE;
	m_shadowViewport.Height = (float)SHADOWMAP_SIZE;
	m_shadowViewport.MinDepth = 0.f;
	m_shadowViewport.MaxDepth = 1.f;
}

void Renderer::CreateDepthStencilView(UINT width, UINT height)
{
	//DepthStencilView 초기화
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> textureDepthStencil = nullptr;
	HR_T(m_pDevice.Get()->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	HR_T(m_pDevice->CreateDepthStencilView(textureDepthStencil.Get(), &descDSV, &m_pDepthStencilView));

	//Shadow DepthStencilView 초기화
	descDepth = {};
	descDepth.Width = (float)(SHADOWMAP_SIZE);
	descDepth.Height = (float)(SHADOWMAP_SIZE);
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;

	HR_T(m_pDevice->CreateTexture2D(&descDepth, NULL, m_pShadowMap.GetAddressOf()));

	descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	HR_T(m_pDevice->CreateDepthStencilView(m_pShadowMap.Get(), &descDSV, m_pShadowMapDSV.GetAddressOf()));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	HR_T(m_pDevice->CreateShaderResourceView(m_pShadowMap.Get(), &srvDesc, m_pShadowMapSRV.GetAddressOf()));
}

DirectX::XMFLOAT3 Renderer::ConvertToNDC(const Vector3D& pos) const
{
	Math::Matrix matrix;
	matrix.Translation(Vector3(pos.GetX(), pos.GetY(), pos.GetZ()));
	matrix = matrix * m_viewMatrix * m_projectionMatrix;
	Vector3 translation = matrix.Translation();
	translation = translation / matrix._44;
	return { (translation.x + 1) * 960.f,540.f * (1 - translation.y), translation.z };
}

const wchar_t* Renderer::ConvertToWchar(const std::string& str) const
{
	const int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	const auto finalText = new wchar_t[bufferSize];

	// string wchar_t로 변환
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, finalText, bufferSize);

	return finalText;
	return nullptr;
}

void Renderer::CreateSamplerState()
{
	//sampler 초기화
	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sd, m_pSampler.GetAddressOf()));

	sd = {};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.MaxAnisotropy = (sd.Filter == D3D11_FILTER_ANISOTROPIC) ? D3D11_REQ_MAXANISOTROPY : 1;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sd, m_pSamplerClamp.GetAddressOf()));

	m_pDeviceContext->PSSetSamplers(1, 1, Renderer::Instance->m_pSamplerClamp.GetAddressOf());
}

void Renderer::FrustumCulling(StaticModel* model)
{
	if (m_frustumCmaera.Intersects(model->m_boundingBox))
	{
		AddMeshInstance(model);
	}
}

void Renderer::ApplyMaterial(Material* pMaterial)
{
	if (pMaterial->m_pDiffuseRV)
		m_pDeviceContext->PSSetShaderResources(0, 1, pMaterial->m_pDiffuseRV->m_pTextureRV.GetAddressOf());

	if (pMaterial->m_pNormalRV)
		m_pDeviceContext->PSSetShaderResources(1, 1, pMaterial->m_pNormalRV->m_pTextureRV.GetAddressOf());

	if (pMaterial->m_pSpecularRV)
		m_pDeviceContext->PSSetShaderResources(2, 1, pMaterial->m_pSpecularRV->m_pTextureRV.GetAddressOf());

	if (pMaterial->m_pEmissiveRV)
		m_pDeviceContext->PSSetShaderResources(3, 1, pMaterial->m_pEmissiveRV->m_pTextureRV.GetAddressOf());

	if (pMaterial->m_pOpacityRV)
		m_pDeviceContext->PSSetShaderResources(4, 1, pMaterial->m_pOpacityRV->m_pTextureRV.GetAddressOf());

	if (pMaterial->m_pMetalicRV)
		m_pDeviceContext->PSSetShaderResources(5, 1, pMaterial->m_pMetalicRV->m_pTextureRV.GetAddressOf());

	if (pMaterial->m_pRoughnessRV)
		m_pDeviceContext->PSSetShaderResources(6, 1, pMaterial->m_pRoughnessRV->m_pTextureRV.GetAddressOf());
	if (pMaterial->m_pAmbientOcclusionRV)
		m_pDeviceContext->PSSetShaderResources(12, 1, pMaterial->m_pAmbientOcclusionRV->m_pTextureRV.GetAddressOf());
}

void Renderer::SphereInit(string filename)
{
	m_pSphere = new StaticModel();
	m_pSphere->Load(filename);
	m_pSphere->m_worldTransform = Math::Matrix::Identity;
}

void Renderer::SphereRender()
{
	m_pDeviceContext->UpdateSubresource(m_pSphereBuffer.Get(), 0, nullptr, &m_sphereCB, 0, 0);
	m_pDeviceContext->PSSetConstantBuffers(5, 1, m_pSphereBuffer.GetAddressOf());
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pWorldBuffer.GetAddressOf());
	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState.Get(), nullptr, 0xffffffff);
	Material* pPrevMaterial = nullptr;

	for (auto it : m_pSphere->m_meshInstance)
	{
		string name = it.m_pMeshResource->m_meshName;
		if (pPrevMaterial != it.m_pMaterial)
		{
			Renderer::Instance->m_pDeviceContext->VSSetShader(it.m_pMeshResource->m_vertexShader.m_pVertexShader.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetShader(m_pSpherePS.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetSamplers(0, 1, Renderer::Instance->m_pSampler.GetAddressOf());

			Renderer::Instance->ApplyMaterial(it.m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = it.m_pMaterial;
		}
		m_pDeviceContext->PSSetShaderResources(7, 1, m_pShadowMapSRV.GetAddressOf());
		m_worldMatrixCB.mWorld = it.m_pNodeWorldTransform.Transpose();
		m_pDeviceContext->UpdateSubresource(m_pWorldBuffer.Get(), 0, nullptr, &m_worldMatrixCB, 0, 0);
		it.Render(Renderer::Instance->m_pDeviceContext.Get());
	}
}

void Renderer::OutlineRender()
{
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pWorldBuffer.GetAddressOf());
	m_pDeviceContext->OMSetDepthStencilState(m_pOutlineDSS.Get(), 1);
	for (auto it : m_pOutlineMesh)
	{
		m_pDeviceContext->VSSetShader(m_pOutlineVS.Get(), nullptr, 0);
		m_pDeviceContext->PSSetShader(m_pOutlinePS.Get(), nullptr, 0);
		m_worldMatrixCB.mWorld = it->m_pNodeWorldTransform.Transpose();
		m_pDeviceContext->UpdateSubresource(m_pWorldBuffer.Get(), 0, nullptr, &m_worldMatrixCB, 0, 0);
		it->Render(m_pDeviceContext.Get());
	}

}

void Renderer::MeshRender()
{
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pWorldBuffer.GetAddressOf());
	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	//m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState.Get(), nullptr, 0xffffffff);
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
	Material* pPrevMaterial = nullptr;

	for (auto it : m_pMeshInstance)
	{
		string name = it->m_pMeshResource->m_meshName;
		if (pPrevMaterial != it->m_pMaterial)
		{
			Renderer::Instance->m_pDeviceContext->VSSetShader(it->m_pMeshResource->m_vertexShader.m_pVertexShader.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetShader(it->m_pMaterial->m_pixelShader.m_pPixelShader.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetSamplers(0, 1, Renderer::Instance->m_pSampler.GetAddressOf());

			Renderer::Instance->ApplyMaterial(it->m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = it->m_pMaterial;
		}
		m_pDeviceContext->PSSetShaderResources(7, 1, m_pShadowMapSRV.GetAddressOf());
		m_worldMatrixCB.mWorld = it->m_pNodeWorldTransform.Transpose();
		m_pDeviceContext->UpdateSubresource(m_pWorldBuffer.Get(), 0, nullptr, &m_worldMatrixCB, 0, 0);
		it->Render(Renderer::Instance->m_pDeviceContext.Get());
	}
}
void  Renderer::OpacityMeshRender()
{
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pWorldBuffer.GetAddressOf());
	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());
	//m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState.Get(), nullptr, 0xffffffff);
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
	Material* pPrevMaterial = nullptr;

	for (auto it : m_pOpacityInstance)
	{
		string name = it->m_pMeshResource->m_meshName;
		if (pPrevMaterial != it->m_pMaterial)
		{
			Renderer::Instance->m_pDeviceContext->VSSetShader(it->m_pMeshResource->m_vertexShader.m_pVertexShader.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetShader(it->m_pMaterial->m_pixelShader.m_pPixelShader.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetSamplers(0, 1, Renderer::Instance->m_pSampler.GetAddressOf());

			Renderer::Instance->ApplyMaterial(it->m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = it->m_pMaterial;
		}
		m_pDeviceContext->PSSetShaderResources(7, 1, m_pShadowMapSRV.GetAddressOf());
		m_worldMatrixCB.mWorld = it->m_pNodeWorldTransform.Transpose();
		m_pDeviceContext->UpdateSubresource(m_pWorldBuffer.Get(), 0, nullptr, &m_worldMatrixCB, 0, 0);
		it->Render(Renderer::Instance->m_pDeviceContext.Get());
	}
}

void Renderer::ShadowRender()
{
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pWorldBuffer.GetAddressOf());

	Material* pPrevMaterial = nullptr;
	for (auto it : m_pMeshInstance)
	{
		if (pPrevMaterial != it->m_pMaterial)
		{
			Renderer::Instance->m_pDeviceContext->VSSetShader(m_pShadowVS.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetShader(m_pShadowPS.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetSamplers(1, 1, Renderer::Instance->m_pSampler.GetAddressOf());

			Renderer::Instance->ApplyMaterial(it->m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = it->m_pMaterial;
		}
		m_worldMatrixCB.mWorld = it->m_pNodeWorldTransform.Transpose();
		m_pDeviceContext->UpdateSubresource(m_pWorldBuffer.Get(), 0, nullptr, &m_worldMatrixCB, 0, 0);
		it->Render(Renderer::Instance->m_pDeviceContext.Get());
	}
}

void Renderer::MakeModelEmpty()
{
	for (auto& model : m_pStaticModels)
	{
		if (model->GetSceneResource() != nullptr)
		{
			model->SetSceneResource(nullptr);
			model->m_meshInstance.clear();
			model->m_boundingBox = {};
			model->m_worldTransform = Math::Matrix{};
		}
	}
}

void Renderer::RenderDebugDraw()
{
	m_pDeviceContext->OMSetBlendState(DebugDraw::g_States->Opaque(), nullptr, 0xFFFFFFFF);
	//m_pDeviceContext->RSSetState(DebugDraw::g_States->CullNone());

	DebugDraw::g_BatchEffect->Apply(m_pDeviceContext.Get());
	DebugDraw::g_BatchEffect->SetView(m_viewMatrix);
	DebugDraw::g_BatchEffect->SetProjection(m_projectionMatrix);

	m_pDeviceContext->IASetInputLayout(DebugDraw::g_pBatchInputLayout.Get());

	DebugDraw::g_Batch->Begin();

#ifdef _DEBUG
	for (auto& model : m_boundingBox)
	{
		DebugDraw::Draw(DebugDraw::g_Batch.get(), model,
			 Colors::Blue);
	}
#endif
	for (auto& box : m_colliderBox)
	{
		DebugDraw::Draw(DebugDraw::g_Batch.get(), box.colliderBox, Colors::Green);
	}
	m_colliderBox.clear();
	DebugDraw::g_Batch->End();
}

void Renderer::RenderQueueSort()
{
	m_pOpacityInstance.sort([](const StaticMeshInstance* lhs, const StaticMeshInstance* rhs) {
		return lhs->m_pMaterial < rhs->m_pMaterial;
		});
	m_pMeshInstance.sort([](const StaticMeshInstance* lhs, const StaticMeshInstance* rhs) {
		return lhs->m_pMaterial < rhs->m_pMaterial;
		});
}

void Renderer::SetEnvironment(std::string filename)
{
	auto it = ResourceManager::Instance->m_pOriginalEnvironments.find(filename);
	if (it != ResourceManager::Instance->m_pOriginalEnvironments.end())
	{
		auto pEnvironment = it->second;
		m_pDeviceContext->PSSetShaderResources(8, 1, pEnvironment->m_pEnvironmentTextureResource->m_pTextureRV.GetAddressOf());
		m_pDeviceContext->PSSetShaderResources(9, 1, pEnvironment->m_pIBLDiffuseTextureResource->m_pTextureRV.GetAddressOf());
		m_pDeviceContext->PSSetShaderResources(10, 1, pEnvironment->m_pIBLSpecularTextureResource->m_pTextureRV.GetAddressOf());
		m_pDeviceContext->PSSetShaderResources(11, 1, pEnvironment->m_pIBLBRDFTextureResource->m_pTextureRV.GetAddressOf());
	}
}

void Renderer::Update()
{
	//라이트 방향 업데이트
	m_pDeviceContext->UpdateSubresource(m_pLightBuffer.Get(), 0, nullptr, &m_lightCB, 0, 0);
	m_pDeviceContext->VSSetConstantBuffers(3, 1, m_pLightBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(3, 1, m_pLightBuffer.GetAddressOf());

	//뷰 매트릭스(카메라) 업데이트
	m_viewMatrix = DirectX::XMMatrixLookToLH(m_cameraPos, m_cameraEye, m_cameraUp);

	//m_cameraEye = m_cameraEye - m_cameraEye.Forward;
	//m_viewMatrix = DirectX::XMMatrixLookAtLH(m_cameraPos, m_cameraEye, m_cameraUp);

	m_viewMatrixCB.mView = m_viewMatrix.Transpose();

	//그림자 View, Projection 매트릭스 생성
	Matrix shadowProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, SHADOWMAP_SIZE / SHADOWMAP_SIZE, 300.0f, 20000.0f);
	Vector3 shadowLookAt = { 0, 0, 0 };
	Vector3 shadowPos = shadowLookAt + (-m_lightCB.mDirection * 1000);
	Matrix shadowView = DirectX::XMMatrixLookAtLH(shadowPos, shadowLookAt, Vector3(0.f, 1.f, 0.f));

	m_shadowDirection = shadowLookAt - shadowPos;
	m_shadowDirection.Normalize();

	m_viewMatrixCB.mShadowView = shadowView.Transpose();
	m_projectionMatrixCB.mShadowProjection = shadowProjection.Transpose();
	m_projectionMatrixCB.mCameraPos = m_cameraPos;

	DirectX::BoundingFrustum::CreateFromMatrix(m_frustumCmaera, m_projectionMatrix);
	m_frustumCmaera.Transform(m_frustumCmaera, m_viewMatrix.Invert());
	/*for (auto& model : m_pStaticModels)
	{
		if (model->GetSceneResource() == nullptr)
			break;
		FrustumCulling(model);
	}*/
	for (auto& model : m_pStaticModels)
	{
		AddMeshInstance(model);
	}

	// pointLight Frustum Culling
	//for (int i = 0; i < pointLightCount; i++)
	//{
	//	DirectX::BoundingBox pointLightBoundingBox;

	//	// Calculate the extents of the bounding box based on the radius
	//	Vector3 extents(m_pointLights[i].GetRadius(), m_pointLights[i].GetRadius(), m_pointLights[i].GetRadius());

	//	// Set the bounding box parameters
	//	pointLightBoundingBox.Center = m_pointLights[i].GetPosition();
	//	pointLightBoundingBox.Extents = extents;

	//	if (m_frustumCmaera.Intersects(pointLightBoundingBox))
	//	{
	//		m_pointLightInstance.push_back(m_pointLights[i]);
	//	}
	//}

	//AddOutlineMesh(m_pStaticModels[1]);
	RenderQueueSort();
}

void Renderer::RenderBegin()
{
	m_projectionMatrixCB.mProjection = m_projectionMatrix.Transpose();

	m_pDeviceContext->UpdateSubresource(m_pProjectionBuffer.Get(), 0, nullptr, &m_projectionMatrixCB, 0, 0);

	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

	/*assert(m_pointLightInstance.size() <= pointLightCount);
	for (int i = 0; i < m_pointLightInstance.size(); i++)
	{
		m_pointLightCB.pointLights[i].mPos = m_pointLightInstance[i].GetPosition();
		m_pointLightCB.pointLights[i].mRadius = m_pointLightInstance[i].GetRadius();
		m_pointLightCB.pointLights[i].mLightColor = m_pointLightInstance[i].GetColor();
		m_pointLightCB.pointLights[i].mIntensity = m_pointLightInstance[i].GetIntensity();
		m_pointLightCB.mConstantTerm = m_pointLightInstance[i].GetConstantTerm();
		m_pointLightCB.mCameraPos = m_cameraPos;
	}*/

	m_pDeviceContext->UpdateSubresource(m_pPointLightBuffer.Get(), 0, nullptr, &m_pointLightCB, 0, 0);

	m_pDeviceContext->VSSetConstantBuffers(4, 1, m_pPointLightBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(4, 1, m_pPointLightBuffer.GetAddressOf());
	Clear(0, 1, 0);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::RenderText() const
{
	for (int i = 0; i < m_texts.size(); i++)
	{
		const wchar_t* text = ConvertToWchar(m_texts[i].mText);
		m_spriteFont->DrawString(m_spriteBatch.get(), text, m_texts[i].mPosition, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), 0.5f);

		delete[] text;
	}

	for (int i = 0; i < m_dynamicTexts.size(); i++)
	{
		if (m_dynamicTexts[i].mEnable)
		{
			const wchar_t* text = m_dynamicTexts[i].mText[m_dynamicTexts[i].mIndex].c_str();
			m_spriteFont->DrawString(m_spriteBatch.get(), text, { 960.f, 540.f }, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), 1.5f);
		}
	}
	string Memory;
	GetVideoMemoryInfo(Memory);
	const wchar_t* videoMemory = ConvertToWchar(Memory);
	m_spriteFont->DrawString(m_spriteBatch.get(), videoMemory, DirectX::XMFLOAT2(0.f, 0.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), 0.7f);
	delete[] videoMemory;

	GetSystemMemoryInfo(Memory);
	const wchar_t* systemMemory = ConvertToWchar(Memory);
	m_spriteFont->DrawString(m_spriteBatch.get(), systemMemory, DirectX::XMFLOAT2(0.f, 20.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), 0.7f);
	delete[] systemMemory;

	float FPS = TimeManager::GetInstance()->GetFPS();
	std::string strFPS = "Frame per Second : " + std::to_string(FPS);
	const wchar_t* wFPS = ConvertToWchar(strFPS);
	m_spriteFont->DrawString(m_spriteBatch.get(), wFPS, DirectX::XMFLOAT2(0.f, 40.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), 0.7f);
	delete[] wFPS;

	std::string mousePos = "Mouse Position x : " + std::to_string(InputManager::GetInstance()->GetMousePos().x) + " y : " + std::to_string(InputManager::GetInstance()->GetMousePos().y);
	const wchar_t* wMousePos = ConvertToWchar(mousePos);
	m_spriteFont->DrawString(m_spriteBatch.get(), wMousePos, XMFLOAT2(0.f, 60.f), Colors::White, 0.f, XMFLOAT2(0.f, 0.f), 0.7f);
}

void Renderer::RenderSprite() const
{
	/*std::sort(m_sprites.begin(), m_sprites.end(), [](const SpriteInformation& lhs, const SpriteInformation& rhs)
		{
			return lhs.mLayer < rhs.mLayer;
		});*/

	for (const auto& it : m_sprites)
	{
		if(WorldManager::GetInstance()->GetCurrentWorld() == it.world)
		{
			m_spriteBatch->Draw(it.mSprite.Get(), it.mPosition, nullptr, DirectX::Colors::White, 0.f, XMFLOAT2(0, 0), XMFLOAT2(1, 1), SpriteEffects_None, it.mLayer);
		}
	}

	m_spriteBatch->End();

}



void Renderer::GameAppRender()
{
	//그림자 맵 생성
	m_pDeviceContext->RSSetViewports(1, &m_shadowViewport);
	m_pDeviceContext->OMSetRenderTargets(0, NULL, m_pShadowMapDSV.Get());
	m_pDeviceContext->ClearDepthStencilView(m_pShadowMapDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->PSSetShader(NULL, NULL, 0);

	//그림자의 View, Projection 포함하여 버퍼에 업데이트
	m_pDeviceContext->UpdateSubresource(m_pViewBuffer.Get(), 0, nullptr, &m_viewMatrixCB, 0, 0);
	m_pDeviceContext->VSSetConstantBuffers(1, 1, m_pViewBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(1, 1, m_pViewBuffer.GetAddressOf());

	m_pDeviceContext->UpdateSubresource(m_pProjectionBuffer.Get(), 0, nullptr, &m_projectionMatrixCB, 0, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());

	//그림자 렌더
	ShadowRender();


	//뷰포트와 뎁스 스텐실 뷰를 카메라 기준으로 변경
	Clear();
	//m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);		// Clear() 함수에 이미 있는디?
	m_pDeviceContext->RSSetViewports(1, &m_viewport);

	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());		// 실제 게임 렌더타겟에 렌더

	//메쉬 렌더
	RenderEnvironment();
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
	SphereRender();

	OutlineRender();
	MeshRender();
	OpacityMeshRender();


	RenderDebugDraw();


	m_spriteBatch->Begin();
	RenderText();
	RenderSprite();
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);


	//임구이 렌더
	//RenderImgui();
}

void Renderer::EditorRender()
{
	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	m_RenderTexture->SetRenderTarget(m_pDeviceContext.Get(), m_pDepthStencilView.Get());

	// 렌더링을 텍스처에 지웁니다.
	m_RenderTexture->ClearRenderTarget(m_pDeviceContext.Get(), m_pDepthStencilView.Get(), 0.5f, 0.5f, 0.5f, 1.0f);

	//그림자 맵 생성
	m_pDeviceContext->RSSetViewports(1, &m_shadowViewport);
	m_pDeviceContext->OMSetRenderTargets(0, NULL, m_pShadowMapDSV.Get());
	m_pDeviceContext->ClearDepthStencilView(m_pShadowMapDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->PSSetShader(NULL, NULL, 0);

	//그림자의 View, Projection 포함하여 버퍼에 업데이트
	m_pDeviceContext->UpdateSubresource(m_pViewBuffer.Get(), 0, nullptr, &m_viewMatrixCB, 0, 0);
	m_pDeviceContext->VSSetConstantBuffers(1, 1, m_pViewBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(1, 1, m_pViewBuffer.GetAddressOf());

	m_pDeviceContext->UpdateSubresource(m_pProjectionBuffer.Get(), 0, nullptr, &m_projectionMatrixCB, 0, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());

	//그림자 렌더
	ShadowRender();

	//뷰포트와 뎁스 스텐실 뷰를 카메라 기준으로 변경
	Clear();
	//m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);		// Clear() 함수에 이미 있는디?
	m_pDeviceContext->RSSetViewports(1, &m_viewport);


	m_RenderTexture->SetRenderTarget(m_pDeviceContext.Get(), m_pDepthStencilView.Get());							// 에디터용 렌더타겟에 렌더

	//메쉬 렌더
	RenderEnvironment();
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
	//SphereRender();
	MeshRender();


	RenderDebugDraw();



	m_spriteBatch->Begin();
	//RenderText();
	RenderSprite();
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);


	//임구이 렌더
	//RenderImgui();

	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
}




void Renderer::RenderEnvironment()
{
	m_pDeviceContext->PSSetShader(m_pEnvironmentPS.Get(), nullptr, 0);

	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pWorldBuffer.GetAddressOf());
	m_pDeviceContext->RSSetState(m_pRasterizerStateCCW.Get());
	m_pDeviceContext->OMSetDepthStencilState(m_pSkyboxDSS.Get(), 0);
	m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	auto test = ResourceManager::Instance->m_pOriginalEnvironments["xiequ_yuan_4k"];
	m_worldMatrixCB.mWorld = ResourceManager::Instance->m_pOriginalEnvironments["xiequ_yuan_4k"]->m_worldTransform.Transpose();
	m_pDeviceContext->UpdateSubresource(m_pWorldBuffer.Get(), 0, nullptr, &m_worldMatrixCB, 0, 0);
	ResourceManager::Instance->m_pOriginalEnvironments["xiequ_yuan_4k"]->m_meshInstance.Initialize();
	ResourceManager::Instance->m_pOriginalEnvironments["xiequ_yuan_4k"]->m_meshInstance.Render(m_pDeviceContext.Get());

}



void Renderer::RenderEnd()
{
	m_pSwapChain->Present(0, 0);
	m_pMeshInstance.clear();
	m_pOpacityInstance.clear();
	m_pOutlineMesh.clear();
	m_pointLightInstance.clear();
	m_boundingBox.clear();
	MakeModelEmpty();
}

bool Renderer::InitImgui(HWND hWnd)
{
	// ImGui 초기화
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ImGui 스타일 설정
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// 플랫폼, 렌더러 설정
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get());

	return true;
}

void Renderer::UnInitImgui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Renderer::CreateShadowVS()
{
	// minjeong : CreateShadowVS
	ID3D10Blob* vertexShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"../Resource/ShadowVertexShader.hlsl", 0, "main", "vs_5_0", &vertexShaderBuffer));
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, m_pShadowVS.GetAddressOf()));
}

void Renderer::CreateShadowPS()
{
	// minjeong : CreateShadowPS
	ID3D10Blob* pixelShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"../Resource/ShadowDepthPS.hlsl", 0, "main", "ps_5_0", &pixelShaderBuffer));
	HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, m_pShadowPS.GetAddressOf()));
}

void Renderer::RenderImgui()
{
	// minjeong : Debug Shader Window
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Camera Property
	{
		ImGui::Begin("Camera Properties");
		float x = DirectX::XMVectorGetX(m_cameraPos);
		float y = DirectX::XMVectorGetY(m_cameraPos);
		float z = DirectX::XMVectorGetZ(m_cameraPos);
		ImGui::Text("Position");
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SliderFloat("##cpx", &x, -1000.f, 1000.f);
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SliderFloat("##cpy", &y, -1000.f, 1000.f);
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::SliderFloat("##cpz", &z, -1000.f, 1000.f);
		m_cameraPos = DirectX::XMVectorSet(x, y, z, 0.0f);
		ImGui::End();
	}

	// Light Property
	{
		// Directional Light
		ImGui::Begin("Light Properties");
		ImGui::Text("Direction Light Dir");
		ImGui::Text("Direction Light Dir");
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SliderFloat("##lpx", &m_lightCB.mDirection.x, -1.f, 1.f);
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SliderFloat("##lpy", &m_lightCB.mDirection.y, -1.f, 1.f);
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::SliderFloat("##lpz", &m_lightCB.mDirection.z, -1.f, 1.f);

		//// Point Light
		//ImGui::Text("Point Light Index");
		//ImGui::SliderInt("##plIndex", &m_pointLightIndex, 0, pointLightCount - 1);
		//ImGui::Text("Point Light Pos");
		//Vector3 pointLightPos = m_pointLights[m_pointLightIndex].GetPosition();
		//ImGui::Text("X");
		//ImGui::SameLine();
		//ImGui::SliderFloat("##plx", &pointLightPos.x, -1000.f, 1000.f);
		//ImGui::Text("Y");
		//ImGui::SameLine();
		//ImGui::SliderFloat("##ply", &pointLightPos.y, -1000.f, 1000.f);
		//ImGui::Text("Z");
		//ImGui::SameLine();
		//ImGui::SliderFloat("##plz", &pointLightPos.z, -1000.f, 1000.f);
		//m_pointLights[m_pointLightIndex].SetPosition(pointLightPos);

		//float pointLightIntensity = m_pointLights[m_pointLightIndex].GetIntensity();
		//ImGui::Text("Intensity");
		//ImGui::SameLine();
		//ImGui::SliderFloat("##pis", &pointLightIntensity, 0.f, 10000.f);
		//m_pointLights[m_pointLightIndex].SetIntensity(pointLightIntensity);
		//if(m_pointLights[m_pointLightIndex].GetIntensity() < 100.f)
		//{
		//	auto test = 1;
		//}

		//float pointLightRadius = m_pointLights[m_pointLightIndex].GetRadius();
		//ImGui::Text("Radius");
		//ImGui::SameLine();
		//ImGui::SliderFloat("##prad", &pointLightRadius, 0.f, 600.f);
		//m_pointLights[m_pointLightIndex].SetRadius(pointLightRadius);

		//float pointLightColor[3] = { m_pointLights[m_pointLightIndex].GetColor().x, m_pointLights[m_pointLightIndex].GetColor().y, m_pointLights[m_pointLightIndex].GetColor().z };
		//ImGui::Text("Color");
		//ImGui::SameLine();
		//ImGui::ColorEdit3("##plc", pointLightColor, 0);
		//m_pointLights[m_pointLightIndex].SetColor(pointLightColor[0], pointLightColor[1], pointLightColor[2]);
		{
			bool useIBL = m_sphereCB.mUseIBL;
			ImGui::Begin("IBL");
			ImGui::Text("IBL");
			ImGui::Text("Metalic");
			ImGui::SameLine();
			ImGui::SliderFloat("##lpx", &m_sphereCB.mMetalic, 0.f, 1.f);
			ImGui::Text("Roughness");
			ImGui::SameLine();
			ImGui::SliderFloat("##lpy", &m_sphereCB.mRoughness, 0.f, 1.f);
			ImGui::Text("Ambient");
			ImGui::SameLine();
			ImGui::SliderFloat("##lpz", &m_sphereCB.mAmbient, 0.f, 1.f);
			ImGui::Checkbox("UseIBL", &useIBL);
			m_sphereCB.mUseIBL = useIBL;
			ImGui::End();
		}
		// Shadow
		ImGui::Text("Shadow");
		ImGui::Image(m_pShadowMapSRV.Get(), ImVec2(256, 256));
		std::string str = std::to_string(m_shadowDirection.x) + ", " + std::to_string(m_shadowDirection.y) + ", " + std::to_string(m_shadowDirection.z);
		ImGui::Text("ShadowDirection : %s", str.c_str());
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::GetVideoMemoryInfo(std::string& out) const
{
	DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
	m_pDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);
	out = "Video Memory : " + std::to_string(videoMemoryInfo.CurrentUsage / 1024 / 1024) + " MB" + " / " + std::to_string(videoMemoryInfo.Budget / 1024 / 1024) + " MB";
}

void Renderer::GetSystemMemoryInfo(std::string& out) const
{
	HANDLE hProcess = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX pmc;
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	out = "System Memory : " + std::to_string((pmc.PagefileUsage) / 1024 / 1024) + " MB";
}

void Renderer::SetCamera(Math::Matrix matrix)
{
	m_cameraPos = matrix.Translation();
	m_cameraEye = matrix.Forward();
	m_cameraUp = matrix.Up();
}

bool Renderer::Initialize(HWND* hWnd, UINT width, UINT height)
{
	HRESULT hr = 0;

	HR_T(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)m_pDXGIFactory.GetAddressOf()));
	HR_T(m_pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(m_pDXGIAdapter.GetAddressOf())));

	//스왑 체인 정보
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferCount = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = *hWnd;


	swapDesc.Windowed = true;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;


	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;


	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;

	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	UINT creationFlags = 0;

#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//디바이스, 스왑체인, 디바이스 컨텍스트 생성
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL, D3D11_SDK_VERSION, &swapDesc, m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), NULL, m_pDeviceContext.GetAddressOf()));

	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_pDevice.Get()
		, m_fontFilePath);
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pDeviceContext.Get());

	//렌더타겟 뷰 생성
	ComPtr<ID3D11Texture2D> pBackBufferMaterial = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferMaterial));
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferMaterial.Get(), nullptr, m_pRenderTargetView.GetAddressOf()));




	//뷰포트, 뎁스 스텐실 뷰, 샘플러 상태 설정
	CreateViewport(width, height);
	CreateDepthStencilView(width, height);
	CreateSamplerState();

	//월드 상수버퍼
	D3D11_BUFFER_DESC worldbd;
	worldbd.Usage = D3D11_USAGE_DEFAULT;
	worldbd.ByteWidth = sizeof(cbWorld);
	worldbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	worldbd.CPUAccessFlags = 0;
	worldbd.MiscFlags = 0;
	HR_T(Renderer::Instance->m_pDevice->CreateBuffer(&worldbd, nullptr, &m_pWorldBuffer));

	//뷰 상수버퍼
	D3D11_BUFFER_DESC viewbd;
	viewbd.Usage = D3D11_USAGE_DEFAULT;
	viewbd.ByteWidth = sizeof(cbView);
	viewbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	viewbd.CPUAccessFlags = 0;
	viewbd.MiscFlags = 0;
	HR_T(Renderer::Instance->m_pDevice->CreateBuffer(&viewbd, nullptr, &m_pViewBuffer));

	//프로젝션 상수버퍼
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(cbProjection);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, &m_pProjectionBuffer));
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, width / (FLOAT)height, 10.f, 10000.0f);
	m_projectionMatrixCB.mProjection = m_projectionMatrix.Transpose();

	//라이트 상수버퍼
	D3D11_BUFFER_DESC lightbd;
	lightbd.Usage = D3D11_USAGE_DEFAULT;
	lightbd.ByteWidth = sizeof(cbLight);
	lightbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightbd.CPUAccessFlags = 0;
	lightbd.MiscFlags = 0;
	HR_T(Renderer::Instance->m_pDevice->CreateBuffer(&lightbd, nullptr, &m_pLightBuffer));

	// minjeong : Create VS & PS
	CreateShadowPS();
	CreateShadowVS();

	//뎁스 스텐실 스테이트 초기화
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = true;
	dsd.StencilEnable = false;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	HR_T(m_pDevice->CreateDepthStencilState(&dsd, m_pDepthStencilState.GetAddressOf()));

	dsd = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	HR_T(m_pDevice->CreateDepthStencilState(&dsd, m_pSkyboxDSS.GetAddressOf()));

	dsd = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
	dsd.DepthEnable = false;
	dsd.StencilEnable = true;
	dsd.StencilWriteMask = 0xff;
	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;

	HR_T(m_pDevice->CreateDepthStencilState(&dsd, m_pOutlineDSS.GetAddressOf()));
	//래스터라이저 스테이트 초기화
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.AntialiasedLineEnable = true;
	rasterizerDesc.MultisampleEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	HR_T(m_pDevice->CreateRasterizerState(&rasterizerDesc, m_pRasterizerState.GetAddressOf()));

	rasterizerDesc.FrontCounterClockwise = true;
	HR_T(m_pDevice->CreateRasterizerState(&rasterizerDesc, m_pRasterizerStateCCW.GetAddressOf()));

	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);

	DirectX::BoundingFrustum::CreateFromMatrix(m_frustumCmaera, m_projectionMatrix);

	DebugDraw::Initialize(m_pDevice, m_pDeviceContext);

	D3D11_BUFFER_DESC pbd = {};
	pbd.Usage = D3D11_USAGE_DEFAULT;
	pbd.ByteWidth = sizeof(cbPointLight);
	pbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pbd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&pbd, nullptr, m_pPointLightBuffer.GetAddressOf()));
	
	pbd = {};
	pbd.Usage = D3D11_USAGE_DEFAULT;
	pbd.ByteWidth = sizeof(cbBall);
	pbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pbd.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pSphereBuffer.GetAddressOf()));

	//포인트 라이트 테스트용
	/*for (int i = 0; i < 5; i++)
	{
		m_pointLights[i].SetPosition(Vector3(0, 0, 0));
		m_pointLights[i].SetRadius(600.f);
		m_pointLights[i].SetColor();
		m_pointLights[i].SetIntensity(1000.f);
	}*/

	SetAlphaBlendState();

	ResourceManager::Instance->CreateEnvironment("xiequ_yuan_4k");
	SetEnvironment("xiequ_yuan_4k");
	ComPtr < ID3DBlob> buffer;

	buffer.Reset();
	HR_T(CompileShaderFromFile(L"../Resource/PS_Environment.hlsl", nullptr, "main", "ps_5_0", buffer.GetAddressOf()));
	HR_T(m_pDevice->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL, m_pEnvironmentPS.GetAddressOf()));

	buffer.Reset();
	HR_T(CompileShaderFromFile(L"../Resource/VS_Outline.hlsl", nullptr, "main", "vs_5_0", buffer.GetAddressOf()));
	HR_T(m_pDevice->CreateVertexShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL, m_pOutlineVS.GetAddressOf()));

	buffer.Reset();
	HR_T(CompileShaderFromFile(L"../Resource/PS_Outline.hlsl", nullptr, "main", "ps_5_0", buffer.GetAddressOf()));
	HR_T(m_pDevice->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL, m_pOutlinePS.GetAddressOf()));

	buffer.Reset();
	HR_T(CompileShaderFromFile(L"../Resource/BallPBR.hlsl", nullptr, "main", "ps_5_0", buffer.GetAddressOf()));
	HR_T(m_pDevice->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), NULL, m_pSpherePS.GetAddressOf()));

	SphereInit("FBXLoad_Test/fbx/8Ball.fbx");


	// 렌더링 텍스처 객체를 생성한다.
	m_RenderTexture = new RenderTextureClass;
	// 렌더링 텍스처 객체를 초기화한다.
	m_RenderTexture->Initialize(m_pDevice.Get(), width, height);


	Matrix cameraInitPos = Matrix::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(180.f), DirectX::XMConvertToRadians(0.f), DirectX::XMConvertToRadians(0.f)) * Matrix::CreateTranslation(0, 150, -250);
	SetCamera(cameraInitPos);


	/*if (!InitImgui(*hWnd))
		return false;*/


	return true;
}

void Renderer::UnInitialize()
{
	for (int i = 0; i < m_pStaticModels.size(); i++)
	{
		delete m_pStaticModels[i];
	}
	m_pStaticModels.clear();
	m_pStaticModels.shrink_to_fit();
}

void Renderer::SetAlphaBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;

	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0] = rtBlendDesc;
	HR_T(m_pDevice->CreateBlendState(&blendDesc, m_pAlphaBlendState.GetAddressOf()));
}
