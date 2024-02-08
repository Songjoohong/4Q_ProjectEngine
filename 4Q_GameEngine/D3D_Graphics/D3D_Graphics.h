#pragma once
#include "pch.h"
#include "PointLight.h"

class RenderTextureClass;
class StaticMeshResource;
class StaticModel;
class Material;
class StaticMeshInstance;


const size_t BUFFER_SIZE = 2;

struct cbPointLight
{
	Math::Vector3 mPos;
	float mRadius = 600.f;
	Math::Vector3 mLightColor;
	float mLinearTerm = 0.007f;
	Math::Vector3 mCameraPos;
	float mQuadraticTerm = 0.0002f;
	float mIntensity = 1.0f;
	Math::Vector3 mPad0;
};

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

struct DebugInformation
{
	int entityID;
	string mText;
	DirectX::XMFLOAT2 mPosition;
	float depth;
};

struct SpriteInformation
{
	int mEntityID;
	float mLayer;
	bool IsRendered;
	DirectX::XMFLOAT2 mPosition;
	ComPtr<ID3D11ShaderResourceView> mSprite;
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
	ComPtr<ID3D11DepthStencilState>m_pDepthStencilState = nullptr;	//뎁스 스텐실 스테이트
	ComPtr<ID3D11DepthStencilState>m_pGizmoDSS = nullptr;	//뎁스 스텐실 스테이트
	ComPtr<ID3D11SamplerState> m_pSampler = nullptr;				//샘플러
	ComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;

	// minjeong : shadow Interface
	ComPtr<ID3D11VertexShader> m_pShadowVS;
	ComPtr<ID3D11PixelShader> m_pShadowPS;
	ComPtr<ID3D11Texture2D> m_pShadowMap;
	ComPtr<ID3D11DepthStencilView> m_pShadowMapDSV;
	ComPtr<ID3D11ShaderResourceView> m_pShadowMapSRV;
	ComPtr<ID3D11SamplerState> m_pShadowSampler;
	D3D11_VIEWPORT m_viewport;
	D3D11_VIEWPORT m_shadowViewport;

	ComPtr<ID3D11Buffer> m_pWorldBuffer = nullptr;
	RenderTextureClass* m_RenderTexture = nullptr;	// 수민 추가.

	ComPtr<ID3D11Buffer> m_pViewBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pProjectionBuffer = nullptr;

	ComPtr<ID3D11Buffer> m_pPointLightBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pLightBuffer = nullptr;

	
	vector<StaticModel*> m_pStaticModels;			//렌더링 할 스태틱 모델 리스트

	list<StaticMeshInstance*>m_pMeshInstance;	//렌더링 할 메쉬 인스턴스 리스트

	D3D11_VIEWPORT m_baseViewport;

	//spritefont 렌더용
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//빛 테스트용
	PointLight m_pointLight;
	cbPointLight m_pointLightCB;

	//월드 행렬
	Math::Matrix m_worldMatrix;
	cbWorld m_worldMatrixCB;

	//카메라 행렬
	Math::Vector3 m_cameraPos, m_cameraEye = { 0.f,0.f,1.f }, m_cameraUp = { 0.f,1.f,0.f };
	Math::Matrix m_viewMatrix;
	cbView m_viewMatrixCB;

	//프로젝션 행렬
	Math::Matrix m_projectionMatrix;
	cbProjection m_projectionMatrixCB;

	// minejong : directional light constant buffer
	cbLight m_lightCB;
	// minejong : shadow dir
	Vector3 m_shadowDirection;

	DirectX::BoundingFrustum m_frustumCmaera;

public:
	//d3d객체 초기화
	bool Initialize(HWND* Hwnd, UINT Width, UINT Height);

	void UnInitialize();


	//화면 클리어
	void Clear(float r=0.3,float g=1,float b=0.3);

	void Clear(Math::Vector3 color);

	//리소스 경로 설정 및 리턴
	void SetPath(string filePath) { BasePath = filePath; }
	string GetPath() { return BasePath; }

	//빈 모델에 정보 입력
	void AddStaticModel(string filename, const Math::Matrix& worldTM);

	//메쉬 인스턴스 렌더큐에 추가
	void AddMeshInstance(StaticModel* model);

	//디버그 정보 추가
	void AddDebugInformation(int id, const std::string& text, const Vector3D& position);
	void AddSpriteInformation(int id, const std::string& filePath, const DirectX::XMFLOAT2 position, float layer);

	// 디버그 정보 수정
	void EditDebugInformation(int id, const std::string& text, const Vector3D& position);
	void EditSpriteInformation(int id, bool isRendered);

	void DeleteDebugInformation(int id);
	void DeleteSpriteInformation(int id);

	//모델 만들어서 모델 리스트에 추가
	void CreateModel(string filename);


	void CreateViewport(UINT width, UINT height);
	void CreateDepthStencilView(UINT width, UINT height);
	void CreateSamplerState();

	void GetVideoMemoryInfo(std::string& out) const;
	void GetSystemMemoryInfo(std::string& out) const;

	//월드 좌표 ndc로 변환
	DirectX::XMFLOAT3 ConvertToNDC(const Vector3D& pos) const;

	const wchar_t* ConvertToWchar(const string& str) const;


	void FrustumCulling(StaticModel* model);

	void SetCamera(Math::Matrix matrix);

	void ApplyMaterial(Material* pMaterial);

	Math::Matrix GetViewMatrix() { return m_viewMatrix; }

	Math::Matrix GetProjectionMatrix() { return m_projectionMatrix; }
	

	//메쉬 렌더큐에 들어온 메쉬 렌더
	void MeshRender();
	void ShadowRender();


	void Update();

	void RenderText() const;
	void RenderSprite() const;
	void MakeModelEmpty();

	void RenderDebugDraw();


	void RenderBegin();
	void Render();
	void RenderScene();	// 수민
	void RenderToTexture();	// 수민
	void RenderEnd();
	bool InitImgui(HWND hWnd);
	void RenderImgui();
	void UnInitImgui();

	// minjeong : Create Shadow VS & PS
	void CreateShadowVS();
	void CreateShadowPS();private:
	string BasePath = "../Resource/";
	const wchar_t* m_fontFilePath = L"../Resource/font/bitstream.spritefont";
	vector<DebugInformation> m_debugs;
	vector<SpriteInformation> m_sprites;
};