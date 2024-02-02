﻿#include "pch.h"
#include "D3D_Graphics.h"
#include "StaticMeshResource.h"
#include "ResourceManager.h"
#include "StaticModel.h"
#include "RenderTextureClass.h"
#include "../Engine/TimeManager.h"

Renderer* Renderer::Instance = nullptr;

Renderer::Renderer()
{
	if (nullptr == Instance)
		Instance = this;
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

void Renderer::AddStaticModel(string filename, Math::Vector3& pos, Math::Vector3& rot, Math::Vector3& scale)
{
	Vector4 quaternion = Math::Quaternion::CreateFromYawPitchRoll(rot);
	Math::Matrix worldTM = Math::Matrix::CreateScale(scale) * Math::Matrix::CreateFromQuaternion(quaternion) * Math::Matrix::CreateTranslation(pos);
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

void Renderer::CreateModel(string filename)
{
	ResourceManager::Instance->CreateModel(filename);
	StaticModel* pModel = new StaticModel();
	m_pStaticModels.push_back(pModel);
}


DirectX::XMFLOAT3 Renderer::ConvertToNDC(const Vector3D& pos) const
{
	Math::Matrix matrix;
	matrix.Translation(Vector3(pos.GetX(), pos.GetY(), pos.GetZ()));
	matrix = matrix * m_viewMatrix * m_projectionMatrix;

	return { matrix._41, matrix._42, matrix._43 };
}

const wchar_t* Renderer::ConvertToWchar(const string& str) const
{
	const int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	const auto finalText = new wchar_t[bufferSize];

	// string wchar_t로 변환
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, finalText, bufferSize);

	return finalText;
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


void Renderer::RenderBegin()

{
	DirectX::BoundingFrustum::CreateFromMatrix(m_frustumCmaera, m_projectionMatrix);
	m_frustumCmaera.Transform(m_frustumCmaera, m_viewMatrix.Invert());
	for (auto& model : m_pStaticModels)
	{
		if (model->GetSceneResource() == nullptr)
			break;
		FrustumCulling(model);
	}
	Clear();
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
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
	m_spriteBatch->End();

}

void Renderer::Render()
{
	// 전체 장면을 먼저 텍스처로 렌더링합니다.
	RenderToTexture();

	// 씬을 그리기 위해 버퍼를 지웁니다
	Clear();

	// 백 버퍼의 장면을 정상적으로 렌더링합니다.
	RenderScene();
}

void Renderer::RenderScene()
{
	m_viewMatrix = DirectX::XMMatrixLookToLH(m_cameraPos, m_cameraEye, m_cameraUp);

	m_viewMatrixCB.mView = m_viewMatrix.Transpose();

	m_pDeviceContext->UpdateSubresource(m_pViewBuffer.Get(), 0, nullptr, &m_viewMatrixCB, 0, 0);

	m_pDeviceContext->VSSetConstantBuffers(1, 1, m_pViewBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(1, 1, m_pViewBuffer.GetAddressOf());
	MeshRender();
	RenderText();
}

void Renderer::RenderEnd()
{
	m_pSwapChain->Present(0, 0);
	m_pMeshInstance.clear();
	MakeModelEmpty();
}

void Renderer::RenderToTexture()
{
	// 렌더링 대상을 렌더링에 맞게 설정합니다.
	m_RenderTexture->SetRenderTarget(m_pDeviceContext.Get(), m_pDepthStencilView.Get());

	// 렌더링을 텍스처에 지웁니다.
	m_RenderTexture->ClearRenderTarget(m_pDeviceContext.Get(), m_pDepthStencilView.Get(), 0.5f, 0.5f, 0.5f, 1.0f);

	// 이제 장면을 렌더링하면 백 버퍼 대신 텍스처로 렌더링됩니다.
	RenderScene();

	// 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
}

void Renderer::SetCamera(Math::Vector3 position, Math::Vector3 eye, Math::Vector3 up)
{
	m_cameraPos = position;
	m_cameraEye = eye;
	m_cameraUp = up;
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


bool Renderer::Initialize(HWND* Hwnd, UINT Width, UINT Height)
{
	HRESULT hr = 0;

	HR_T(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)m_pDXGIFactory.GetAddressOf()));
	HR_T(m_pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(m_pDXGIAdapter.GetAddressOf())));

	//스왑 체인 정보
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferCount = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = *Hwnd;

	swapDesc.Windowed = true;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	swapDesc.BufferDesc.Width = Width;
	swapDesc.BufferDesc.Height = Height;

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
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL, D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));

	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_pDevice.Get()
		, m_fontFilePath);
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pDeviceContext.Get());


	//렌더타겟 뷰 생성
	ComPtr<ID3D11Texture2D> pBackBufferMaterial = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferMaterial));
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferMaterial.Get(), nullptr, &m_pRenderTargetView));

	//sampler 초기화
	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sd, &m_pSampler));

	//DepthStencilView 초기화
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = Width;
	descDepth.Height = Height;
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

	//뷰포트 설정
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)Width;
	viewport.Height = (float)Height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	m_pDeviceContext->RSSetViewports(1, &viewport);

	//월드 상수버퍼
	D3D11_BUFFER_DESC worldbd;
	worldbd.Usage = D3D11_USAGE_DEFAULT;
	worldbd.ByteWidth = static_cast<UINT>(sizeof(Math::Matrix));
	worldbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	worldbd.CPUAccessFlags = 0;
	worldbd.MiscFlags = 0;

	HR_T(Renderer::Instance->m_pDevice->CreateBuffer(&worldbd, nullptr, &m_pWorldBuffer));

	//뷰 상수버퍼
	D3D11_BUFFER_DESC viewbd;
	viewbd.Usage = D3D11_USAGE_DEFAULT;
	viewbd.ByteWidth = static_cast<UINT>(sizeof(Math::Matrix));
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
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, Width / (FLOAT)Height, 0.1, 10000);

	m_projectionMatrixCB.mProjcetion = m_projectionMatrix.Transpose();

	m_pDeviceContext->UpdateSubresource(m_pProjectionBuffer.Get(), 0, nullptr, &m_projectionMatrixCB, 0, 0);

	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());

	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);

	DirectX::BoundingFrustum::CreateFromMatrix(m_frustumCmaera, m_projectionMatrix);

	SetCamera();

    // 수민
    m_RenderTexture = new RenderTextureClass;
    m_RenderTexture->Initialize(m_pDevice.Get(), Width, Height);

    return true;
}
