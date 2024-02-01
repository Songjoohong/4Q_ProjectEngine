#pragma once
#include "pch.h"
#include "PointLight.h"

class StaticMeshResource;
class StaticModel;
class Material;
class StaticMeshInstance;

const size_t BUFFER_SIZE = 2;

struct cbPointLight
{
	Math::Vector3 mPos;
	float mRadius;
	Math::Vector3 mLightColor;
	float pad;
	Math::Vector3 mCameraPos;
	float pad2;
};

struct cbWorld
{
	Math::Matrix mWorld;
};

struct cbView
{
	Math::Matrix mView;
};

struct cbProjection
{
	Math::Matrix mProjcetion;
};

struct DebugInformation
{
	int entityID;
	string mText;
	DirectX::XMFLOAT2 mPosition;
	float depth;
};
class Renderer
{
public:
	static Renderer* Instance;
	
	Renderer();
	~Renderer() {  }
public:
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI팩토리
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// 비디오카드 정보에 접근 가능한 인터페이스
	ComPtr<ID3D11Device> m_pDevice = nullptr;						//디바이스
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;			//디바이스컨텍스트
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;					//스왑체인
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;	//렌더 타겟 뷰
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;	//뎁스 스텐실 뷰
	ComPtr<ID3D11DepthStencilState>m_pDepthStencilState = nullptr;	//뎁스 스텐실 스테이트
	ComPtr<ID3D11SamplerState> m_pSampler = nullptr;				//샘플러
	ComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;

	ComPtr<ID3D11PixelShader>ps;


	ComPtr<ID3D11Buffer> m_pWorldBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pViewBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pProjectionBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pPointLightBuffer = nullptr;
	
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
	Math::Vector3 m_cameraPos, m_cameraEye, m_cameraUp;
	Math::Matrix m_viewMatrix;
	cbView m_viewMatrixCB;

	//프로젝션 행렬
	Math::Matrix m_projectionMatrix;
	cbProjection m_projectionMatrixCB;

	DirectX::BoundingFrustum m_frustumCmaera;


	
public:
	//d3d객체 초기화
	bool Initialize(HWND* Hwnd, UINT Width, UINT Height);

	void UnInitialize();


	//화면 클리어
	void Clear(float r=0,float g=0,float b=0);
	void Clear(Math::Vector3 color);

	//리소스 경로 설정 및 리턴
	void SetPath(string filePath) { BasePath = filePath; }
	string GetPath() { return BasePath; }

	//빈 모델에 정보 입력
	void AddStaticModel(string filename, Math::Vector3& pos, Math::Vector3& rot, Math::Vector3& scale);

	//메쉬 인스턴스 렌더큐에 추가
	void AddMeshInstance(StaticModel* model);

	//디버그 정보 추가
	void AddDebugInformation(int id, const std::string& text, const Vector3D& position);

	// 디버그 정보 수정
	void EditDebugInformation(int id, const std::string& text, const Vector3D& position);

	//모델 만들어서 모델 리스트에 추가
	void CreateModel(string filename);

	void GetVideoMemoryInfo(std::string& out) const;
	void GetSystemMemoryInfo(std::string& out) const;

	//월드 좌표 ndc로 변환
	DirectX::XMFLOAT3 ConvertToNDC(const Vector3D& pos) const;

	const wchar_t* ConvertToWchar(const string& str) const;

	void FrustumCulling(StaticModel* model);

	void SetCamera(Math::Vector3 position={500.f,0.f,-100.f},Math::Vector3 eye={0,0,1},Math::Vector3 up = {0,1,0});

	void ApplyMaterial(Material* pMaterial);

	

	//메쉬 렌더큐에 들어온 메쉬 렌더
	void MeshRender();

	void RenderText() const;

	void MakeModelEmpty();

	void RenderDebugDraw();

	void RenderBegin();
	void Render();
	void RenderEnd();
private:
	string BasePath = "../Resource/";
	const wchar_t* m_fontFilePath = L"../Resource/font/bitstream.spritefont";
	vector<DebugInformation> m_debugs;
};