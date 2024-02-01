#pragma once
#include "pch.h"


class StaticMeshResource;
class StaticModel;
class Material;
class StaticMeshInstance;

const size_t BUFFER_SIZE = 2;

struct cbWorld
{
	Math::Matrix mWorld;
};

struct cbView
{
	Math::Matrix mView;
	Math::Matrix mShadowView;
};

struct cbProjection
{
	Math::Matrix mProjection;
	Math::Matrix mShadowProjection;
};

struct cbLight
{
	Vector4 mDirection = {0.f, 0.f, 1.f, 1.f};
};

class Renderer
{
public:
	static Renderer* Instance;
	
	Renderer();
	~Renderer();
public:
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI팩토리
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// 비디오카드 정보에 접근 가능한 인터페이스
	ComPtr<ID3D11Device> m_pDevice = nullptr;						//디바이스
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;			//디바이스컨텍스트
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;					//스왑체인
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;	//렌더 타겟 뷰
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;	//뎁스 스텐실 뷰
	ComPtr<ID3D11SamplerState> m_pSampler = nullptr;				//샘플러

	// shadow 위한 객체
	ComPtr<ID3D11Texture2D> m_pShadowMap;
	ComPtr<ID3D11DepthStencilView> m_pShadowMapDSV;
	ComPtr<ID3D11ShaderResourceView> m_pShadowMapSRV;
	D3D11_VIEWPORT m_viewport;
	D3D11_VIEWPORT m_shadowViewport;

	ComPtr<ID3D11Buffer> m_pWorldBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pViewBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pProjectionBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pLightBuffer = nullptr;
	
	vector<StaticModel*> m_pStaticModels;			//렌더링 할 스태틱 모델 리스트

	list<StaticMeshInstance*>m_pMeshInstance;	//렌더링 할 메쉬 인스턴스 리스트

	//월드 행렬
	Math::Matrix m_worldMatrix;
	cbWorld m_worldMatrixCB;

	//카메라 행렬
	Math::Vector3 m_cameraPos, m_cameraEye, m_cameraUp;
	Math::Matrix m_viewMatrix;
	cbView m_viewMatrixCB;

	//프로젝션 행렬
	Math::Matrix m_projectionMatrix;
	cbProjection m_projectionMatrixCB;

	//라이트
	cbLight m_lightCB;

public:
	//d3d객체 초기화
	bool Initialize(HWND* hWnd, UINT width, UINT height);


	//화면 클리어
	void Clear(float r=1,float g=1,float b=1);
	void Clear(Math::Vector3 color);

	//리소스 경로 설정 및 리턴
	void SetPath(string filePath) { BasePath = filePath; }
	string GetPath() { return BasePath; }

	//빈 모델에 정보 입력
	void AddStaticModel(string filename, Math::Vector3& pos, Math::Vector3& rot, Math::Vector3& scale);

	//메쉬 인스턴스 렌더큐에 추가
	void AddMeshInstance(StaticModel* model);
	
	//모델 만들어서 모델 리스트에 추가
	void CreateModel(string filename);

	void SetViewport(UINT width, UINT height);
	void SetDepthStencilView(UINT width, UINT height);

	StaticModel* LoadStaticModel(string filename);

	void SetCamera(Math::Vector3 position={0,0,-100},Math::Vector3 eye={0,0,1},Math::Vector3 up = {0,1,0});

	void ApplyMaterial(Material* pMaterial);

	void MeshRender();

	void Update();

	void RenderBegin();
	void Render();
	void RenderEnd();

	bool InitImgui(HWND hWnd);
	void RenderImgui();
	void UnInitImgui();
private:
	string BasePath = "../Resource/";
};