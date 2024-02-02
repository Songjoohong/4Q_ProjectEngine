#include "pch.h"
#include "D3D_Graphics.h"
#include "StaticMeshResource.h"
#include "ResourceManager.h"
#include "StaticModel.h"

#include "DebugDraw.h"

#include "../Engine/Debug.h"


#include "../Engine/TimeManager.h"



#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#define SHADOWMAP_SIZE 1024

Renderer* Renderer::Instance = nullptr;

Renderer::Renderer()
{
	if (nullptr == Instance)
		Instance = this;
}

Renderer::~Renderer()
{
	UnInitImgui();
}

void Renderer::Clear(float r, float g, float b)
{
	const float clearColor[4] = { r,g,b,1 };
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
}

void Renderer::Clear(Math::Vector3 color)
{
	const float clearColor[4] = { color.x,color.y,color.z,1 };
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
}

void Renderer::AddStaticModel(string filename, const Math::Matrix& worldTM)
{
	
	for (auto& model : m_pStaticModels)
	{
		if (nullptr == model->GetSceneResource())
		{
			model->m_worldTransform = worldTM;
			model->Load(filename);
			break;
		}
		AddMeshInstance(model);
	}
}

void Renderer::AddMeshInstance(StaticModel* model)
{
	for (auto& mesh : model->m_meshInstance)
	{
		m_pMeshInstance.push_back(&mesh);
	}
}

void Renderer::AddDebugInformation(const int id, const std::string& text, const Vector3D& position)
{

	const DirectX::XMFLOAT3 conversion = ConvertToNDC(position);
	const DirectX::XMFLOAT2 pos = { conversion.x, conversion.y };
	const float depth = conversion.z;

	const DebugInformation newDebug = { id, text, pos, depth };
	m_debugs.push_back(newDebug);
}

void Renderer::AddSpriteInformation(int id, const std::string& filePath, const XMFLOAT2 position, float layer)
{
    ComPtr<ID3D11ShaderResourceView> texture;
    const wchar_t* filePathT = ConvertToWchar(filePath);
    CreateTextureFromFile(m_pDevice.Get(), filePathT, &texture);
    m_sprites.push_back(SpriteInformation{ id, layer, true, position, texture });
}

void Renderer::EditDebugInformation(int id, const std::string& text, const Vector3D& position)
{
	const DirectX::XMFLOAT3 conversion = ConvertToNDC(position);
	const DirectX::XMFLOAT2 pos = { conversion.x, conversion.y };
	const float depth = conversion.z;

	auto it = std::find_if(m_debugs.begin(), m_debugs.end(), [id](const DebugInformation& debug)
		{
			return id == debug.entityID;
		});
	it->mPosition = pos;
	it->depth = depth;
	it->mText = text;
}

void Renderer::EditSpriteInformation(int id, bool isRendered)
{
   const auto it = std::find_if(m_sprites.begin(), m_sprites.end(), [id](const SpriteInformation& sprite)
        {
            return id == sprite.mEntityID;
        });
    it->IsRendered = isRendered;
}

void Renderer::DeleteDebugInformation(int id)
{
    m_debugs.erase(std::find_if(m_debugs.begin(), m_debugs.end(), [id](const DebugInformation& debug)
        {
            return id == debug.entityID;
        }));
}

void Renderer::DeleteSpriteInformation(int id)
{
    m_sprites.erase(std::find_if(m_sprites.begin(), m_sprites.end(), [id](const SpriteInformation& sprite)
        {
            return id == sprite.mEntityID;
        }));
}

void Renderer::CreateModel(string filename)
{
	ResourceManager::Instance->CreateModel(filename);
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
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
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

	return {  matrix._41 + 960.f, 
        (540.f - matrix._42), matrix._43 };
}

const wchar_t* Renderer::ConvertToWchar(const string& str) const
{
	const int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	const auto finalText = new wchar_t[bufferSize];

	// string wchar_t로 변환
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, finalText, bufferSize);

	return finalText;
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
}

void Renderer::FrustumCulling(StaticModel* model)
{
	if (m_frustumCamera.Intersects(model->m_boundingBox))
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
}

void Renderer::MeshRender()
{
	m_pDeviceContext->VSSetConstantBuffers(2, 1, m_pWorldBuffer.GetAddressOf());

	Material* pPrevMaterial = nullptr;
	for (auto it : m_pMeshInstance)
	{
		if (pPrevMaterial != it->m_pMaterial)
		{
			Renderer::Instance->m_pDeviceContext->PSSetShader(it->m_pMaterial->m_pixelShader.m_pPixelShader.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetSamplers(0, 1, Renderer::Instance->m_pSampler.GetAddressOf());

			Renderer::Instance->ApplyMaterial(it->m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = it->m_pMaterial;
		}
		m_worldMatrixCB.mWorld = it->m_pNodeWorldTransform->Transpose();
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
			Renderer::Instance->m_pDeviceContext->PSSetShader(m_pShadowPS.Get(), nullptr, 0);
			Renderer::Instance->m_pDeviceContext->PSSetSamplers(1, 1, Renderer::Instance->m_pSampler.GetAddressOf());

			Renderer::Instance->ApplyMaterial(it->m_pMaterial);	// 머터리얼 적용
			pPrevMaterial = it->m_pMaterial;
		}
		m_worldMatrixCB.mWorld = it->m_pNodeWorldTransform->Transpose();
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

    for (auto& model : m_pStaticModels)
    {
        DebugDraw::Draw(DebugDraw::g_Batch.get(), model->m_boundingBox,
            model->m_bIsCulled ? Colors::Red : Colors::Blue);
    }

    DebugDraw::g_Batch->End();

}

void Renderer::Update()
{
	//라이트 방향 업데이트
	m_pDeviceContext->UpdateSubresource(m_pLightBuffer.Get(), 0, nullptr, &m_lightCB, 0, 0);
	m_pDeviceContext->VSSetConstantBuffers(3, 1, m_pLightBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(3, 1, m_pLightBuffer.GetAddressOf());

	//뷰 매트릭스(카메라) 업데이트
	m_viewMatrix = DirectX::XMMatrixLookToLH(m_cameraPos, m_cameraEye, m_cameraUp);

	m_viewMatrixCB.mView = m_viewMatrix.Transpose();

	//그림자 View, Projection 매트릭스 생성
	Matrix shadowProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, SHADOWMAP_SIZE / SHADOWMAP_SIZE, 300.0f, 20000.0f);
	Vector3 shadowLookAt = { 0, 0, 0 };
	Vector3 shadowPos = shadowLookAt + (m_lightCB.mDirection * 1000);
	Matrix shadowView = DirectX::XMMatrixLookAtLH(shadowPos, shadowLookAt, Vector3(0.f, 1.f, 0.f));

	m_shadowDirection = shadowLookAt - shadowPos;
	m_shadowDirection.Normalize();

	m_viewMatrixCB.mShadowView = shadowView.Transpose();
	m_projectionMatrixCB.mShadowProjection = shadowProjection.Transpose();
}

void Renderer::RenderBegin()
{
    m_projectionMatrixCB.mProjection = m_projectionMatrix.Transpose();

    m_pDeviceContext->UpdateSubresource(m_pProjectionBuffer.Get(), 0, nullptr, &m_projectionMatrixCB, 0, 0);

    m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());
    m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());

    m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

    DirectX::BoundingFrustum::CreateFromMatrix(m_frustumCamera, m_projectionMatrix);
    m_frustumCamera.Transform(m_frustumCamera, m_viewMatrix.Invert());
    for (auto& model : m_pStaticModels)
    {
         if (model->GetSceneResource() == nullptr)
            break;
        FrustumCulling(model);
    }
    m_pointLightCB.mPos = m_pointLight.GetPosition();
    m_pointLightCB.mRadius = m_pointLight.GetRadius();
    m_pointLightCB.mLightColor = m_pointLight.GetColor();
    m_pointLightCB.mCameraPos = m_cameraPos;

    m_pDeviceContext->UpdateSubresource(m_pPointLightBuffer.Get(), 0, nullptr, &m_pointLightCB, 0, 0);

    m_pDeviceContext->VSSetConstantBuffers(4, 1, m_pPointLightBuffer.GetAddressOf());
    m_pDeviceContext->PSSetConstantBuffers(4, 1, m_pPointLightBuffer.GetAddressOf());
    Clear();
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
    m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::RenderText() const
{
	m_spriteBatch->Begin();
	for (int i = 0; i < m_debugs.size(); i++)
	{
		const wchar_t* text = ConvertToWchar(m_debugs[i].mText);
		m_spriteFont->DrawString(m_spriteBatch.get(), text, m_debugs[i].mPosition, DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), 0.5f);

		delete[] text;
	}
	string Memory;
	GetVideoMemoryInfo(Memory);
	const wchar_t* videoMemory = ConvertToWchar(Memory);
	m_spriteFont->DrawString(m_spriteBatch.get(), videoMemory, DirectX::XMFLOAT2(0.f,0.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), 0.7f);
	delete[] videoMemory;

	GetSystemMemoryInfo(Memory);
	const wchar_t* systemMemory = ConvertToWchar(Memory);
	m_spriteFont->DrawString(m_spriteBatch.get(), systemMemory, DirectX::XMFLOAT2(0.f, 20.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), 0.7f);
	delete[] systemMemory;

	float FPS = TimeManager::GetInstance()->GetFPS();
	string strFPS = "Frame per Second : " + std::to_string(FPS);
	const wchar_t* wFPS = ConvertToWchar(strFPS);
	m_spriteFont->DrawString(m_spriteBatch.get(), wFPS, DirectX::XMFLOAT2(0.f, 40.f), DirectX::Colors::White, 0.f, DirectX::XMFLOAT2(0.f, 0.f), 0.7f);
	delete[] wFPS;
	

}

void Renderer::RenderSprite() const
{

    for(const auto& it : m_sprites)
    {
        m_spriteBatch->Draw(it.mSprite.Get(), it.mPosition, nullptr, DirectX::Colors::White, 0.f, XMFLOAT2(0,0), XMFLOAT2(1, 1), SpriteEffects_None, it.mLayer);
    }
    m_spriteBatch->End();
}

void Renderer::Render()
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
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->RSSetViewports(1, &m_viewport);
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pDeviceContext->PSSetShaderResources(7, 1, m_pShadowMapSRV.GetAddressOf());

	//메쉬 렌더
	MeshRender();

	RenderDebugDraw();
	RenderText();
    RenderSprite();
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);

	//임구이 렌더
	RenderImgui();
}

void Renderer::RenderEnd()
{
	m_pSwapChain->Present(0, 0);
	m_pMeshInstance.clear();
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

void Renderer::CreateShadowPS()
{
	ID3D10Blob* pixelShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"../Resource/ShadowDepthPS.hlsl", 0, "main", "ps_5_0", &pixelShaderBuffer));
	HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, m_pShadowPS.GetAddressOf()));
}

void Renderer::RenderImgui()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Camera Property
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

	//Light Property
	{
		ImGui::Begin("Light Properties");
		ImGui::Text("Direction");
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::SliderFloat("##lpx", &m_lightCB.mDirection.x, -1.f, 1.f);
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::SliderFloat("##lpy", &m_lightCB.mDirection.y, -1.f, 1.f);
		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::SliderFloat("##lpz", &m_lightCB.mDirection.z, -1.f, 1.f);
		ImGui::Text("Shadow");
		ImGui::Image(m_pShadowMapSRV.Get(), ImVec2(256, 256));
		string str = to_string(m_shadowDirection.x) + ", " + to_string(m_shadowDirection.y) + ", " + to_string(m_shadowDirection.z);
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
	m_cameraUp = Vector3{0.f, 1.f, 0.f};
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
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, width / (FLOAT)height, 0.1f, 100000.0f);
	m_projectionMatrixCB.mProjection = m_projectionMatrix.Transpose();

	//라이트 상수버퍼
	D3D11_BUFFER_DESC lightbd;
	lightbd.Usage = D3D11_USAGE_DEFAULT;
	lightbd.ByteWidth = sizeof(cbLight);
	lightbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightbd.CPUAccessFlags = 0;
	lightbd.MiscFlags = 0;

	HR_T(Renderer::Instance->m_pDevice->CreateBuffer(&lightbd, nullptr, &m_pLightBuffer));

	//그림자 픽셀쉐이더
	CreateShadowPS();


  
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


    

    m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);

    //래스터라이저 스테이트 초기화
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.AntialiasedLineEnable = true;
    rasterizerDesc.MultisampleEnable = true;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    HR_T(m_pDevice->CreateRasterizerState(&rasterizerDesc, m_pRasterizerState.GetAddressOf()));
    m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);

   
    DirectX::BoundingFrustum::CreateFromMatrix(m_frustumCamera, m_projectionMatrix);

    DebugDraw::Initialize(m_pDevice, m_pDeviceContext);
    
    D3D11_BUFFER_DESC pbd = {};
    pbd.Usage = D3D11_USAGE_DEFAULT;
    pbd.ByteWidth = sizeof(cbPointLight);
    pbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    pbd.CPUAccessFlags = 0;
    HR_T(m_pDevice->CreateBuffer(&bd, nullptr, m_pPointLightBuffer.GetAddressOf()));

    //포인트 라이트 테스트용
    m_pointLight.SetPosition(Vector3(480, 00, 1000));
    m_pointLight.SetRadius(500);
    m_pointLight.SetColor();

    
  
  	//Imgui
	if (!InitImgui(*hWnd))
		return false;
  
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
