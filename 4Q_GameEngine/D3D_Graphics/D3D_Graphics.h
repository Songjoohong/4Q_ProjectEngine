#pragma once
#include "pch.h"
#include "PointLight.h"
#include "../Engine/ECS.h"

class RenderTextureClass;
class StaticMeshResource;
class StaticModel;
class Material;
class StaticMeshInstance;
class Environment;

class Sprite2D;

struct ColliderBox
{
	ColliderBox(Vector3 center, Vector3 extents, Quaternion rotation) {
		colliderBox.Center = center;
		colliderBox.Extents = extents;
		colliderBox.Orientation = rotation;
	}
	DirectX::BoundingOrientedBox colliderBox;

};

const size_t BUFFER_SIZE = 2;

static const int pointLightCount = 5;

struct cbPointLight
{
	float mQuadraticTerm = 0.0002f;
	float mConstantTerm = 0.0f;
	float mLinearTerm = 0.007f;
	float mpad1;
	Math::Vector4 mPad0;
	

	struct
	{
		Math::Vector3 mPos;
		float mRadius;
		Math::Vector3 mLightColor;
		float mIntensity;
	} pointLights[pointLightCount];
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
	Math::Vector3 mCameraPos;
	float pad;
};

struct cbLight
{
	Vector4 mDirection = {0.f, -1.f, 1.f, 1.f};
};

struct cbBall
{
	float mMetalic;
	float mRoughness;
	float mAmbient;
	int mUseIBL;
};

struct TextInformation
{
	int mEntityID;
	string mText;
	XMFLOAT2 mPosition;
	float mDepth;
};

struct DynamicTextInformation
{
	int mEntityID;
	int mIndex;
	bool mEnable = false;
	vector<wstring> mText;
};
struct SpriteInformation
{
	ECS::World* world;
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
	ComPtr<ID3D11RenderTargetView> m_pFirstRenderTargetView = nullptr; 

	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;	//뎁스 스텐실 뷰
	ComPtr<ID3D11DepthStencilView> m_pOutlineDepthStencilView = nullptr;	//뎁스 스텐실 뷰
	ComPtr<ID3D11DepthStencilView> m_pOutlineOriginDSV = nullptr;	//뎁스 스텐실 뷰

	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState = nullptr;	//뎁스 스텐실 스테이트
	ComPtr<ID3D11DepthStencilState> m_pSkyboxDSS = nullptr;
	ComPtr<ID3D11DepthStencilState> m_pOutlineDSS = nullptr;
	ComPtr<ID3D11DepthStencilState> m_pOutlineDSS2 = nullptr;

	ComPtr<ID3D11Texture2D>m_pFirstMap=nullptr;
	ComPtr<ID3D11Texture2D>m_pOutlineMap=nullptr;
	ComPtr<ID3D11Texture2D>m_pOriginMap=nullptr;

	ComPtr<ID3D11ShaderResourceView> m_pFirstMapSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_pOutlineMapSRV = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_pOriginMapSRV = nullptr;

	ComPtr<ID3D11BlendState> m_pAlphaBlendState = nullptr;			//알파 블렌드 스테이트

	ComPtr<ID3D11SamplerState> m_pSampler = nullptr;				//샘플러(linear)
	ComPtr<ID3D11SamplerState> m_pSamplerClamp = nullptr;			//샘플러(clamp)


	ComPtr<ID3D11RasterizerState> m_pRasterizerState = nullptr;
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCCW = nullptr;


	// minjeong : shadow Interface
	ComPtr<ID3D11VertexShader> m_pShadowVS;
	ComPtr<ID3D11PixelShader> m_pShadowPS;
	ComPtr<ID3D11PixelShader> m_pEnvironmentPS;
	ComPtr<ID3D11PixelShader> m_pSpherePS;
	ComPtr<ID3D11Texture2D> m_pShadowMap;
	ComPtr<ID3D11DepthStencilView> m_pShadowMapDSV;
	ComPtr<ID3D11ShaderResourceView> m_pShadowMapSRV;
	ComPtr<ID3D11SamplerState> m_pShadowSampler;
	D3D11_VIEWPORT m_viewport;
	D3D11_VIEWPORT m_shadowViewport;


	
	ComPtr<ID3D11VertexShader> m_pOutlineVS;
	ComPtr<ID3D11PixelShader> m_pOutlinePS;
	ComPtr<ID3D11PixelShader> m_pScreenPS;


	ComPtr<ID3D11Buffer> m_pWorldBuffer = nullptr;
	RenderTextureClass* m_RenderTexture = nullptr;	// 수민 추가.

	ComPtr<ID3D11Buffer> m_pViewBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pProjectionBuffer = nullptr;

	ComPtr<ID3D11Buffer> m_pPointLightBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pLightBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pSphereBuffer = nullptr;

	vector<ColliderBox> m_colliderBox;
	vector<DirectX::BoundingBox> m_boundingBox;


	StaticModel* m_pOutlineModel;
	vector<StaticMeshInstance*> m_pOutlineMesh;		//아웃라인을 그릴 메쉬

	vector<StaticModel*> m_pStaticModels;			//렌더링 할 스태틱 모델 리스트

	list<StaticMeshInstance*>m_pMeshInstance;	//렌더링 할 메쉬 인스턴스 리스트

	list<StaticMeshInstance*>m_pOpacityInstance;	

	D3D11_VIEWPORT m_baseViewport;

	StaticMeshResource* m_pScreenMesh;
	StaticMeshInstance* m_pScreenMeshInstance;

	//spritefont 렌더용
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//빛 테스트용
	vector<PointLight> m_pointLightInstance;		// 컬링된 후의 포인트라이트 인스턴스
	cbPointLight m_pointLightCB;
	int m_pointLightIndex = 0;
	vector<PointLight> m_pointLights;				// 컬링되기 전의 포인트라이트 인스턴스

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

	//스피어 테스트
	cbBall m_sphereCB;
	StaticModel* m_pSphere;

	DirectX::BoundingFrustum m_frustumCmaera;


public:
	DirectX::BoundingFrustum& GetCameraFrustum() { return m_frustumCmaera; }
	//d3d객체 초기화
	bool Initialize(HWND* Hwnd, UINT Width, UINT Height);

	void UnInitialize();

	void SetAlphaBlendState();

	//화면 클리어
	void Clear(float r=0.3,float g=1,float b=0.3);

	void Clear(Math::Vector3 color);

	//리소스 경로 설정 및 리턴
	void SetPath(std::string filePath) { BasePath = filePath; }
	std::string GetPath() { return BasePath; }

	//빈 모델에 정보 입력
	void AddStaticModel(std::string filename, const Math::Matrix& worldTM);
	void AddOutlineModel(std::string filename, const Math::Matrix& worldTM);


	//디버그용 콜라이더 박스
	void AddColliderBox(Vector3 center, Vector3 extents, Vector3 rotation);
	void AddBoundingBox(DirectX::BoundingBox boundingBox);

	void AddColliderBox(Vector3 center, Vector3 extents, Math::Matrix worldTM);


	//메쉬 인스턴스 렌더큐에 추가
	void AddMeshInstance(StaticModel* model);

	void AddOutlineMesh(StaticModel* model);

	//디버그 정보 추가
	void AddTextInformation(int id, const std::string& text, const Vector3D& position);
	void AddSpriteInformation(ECS::World* world, int id, const std::string& filePath, const DirectX::XMFLOAT2 position, float layer);
	void AddDynamicTextInformation(int entId, const vector<std::wstring>& vector);

	// 디버그 정보 수정
	void EditTextInformation(int id, const std::string& text, const Vector3D& position);
	void EditSpriteInformation(int id, Sprite2D& sprite2D);
	void EditDynamicTextInformation(int id, int index, bool enable);

	void DeleteTextInformation(int id);
	void DeleteSpriteInformation(int id);
	void DeleteDynamicTextInformation(int entId);

	//모델 만들어서 모델 리스트에 추가
	void CreateModel(std::string filename,DirectX::BoundingBox& boundingBox);

	void CreateScreenMesh();

	void CreateViewport(UINT width, UINT height);
	void CreateDepthStencilView(UINT width, UINT height);
	void CreateSamplerState();

	void GetVideoMemoryInfo(std::string& out) const;
	void GetSystemMemoryInfo(std::string& out) const;

	//월드 좌표 ndc로 변환
	DirectX::XMFLOAT3 ConvertToNDC(const Vector3D& pos) const;

	const wchar_t* ConvertToWchar(const std::string& str) const;


	void FrustumCulling(StaticModel* model);

	void SetCamera(Math::Matrix matrix);

	void ApplyMaterial(Material* pMaterial);


	//스피어 렌더
	void SphereInit(string filename);
	void SphereRender();

	Math::Matrix GetViewMatrix() { return m_viewMatrix; }

	Math::Matrix GetProjectionMatrix() { return m_projectionMatrix; }

	
	void OutlineRender();

	//메쉬 렌더큐에 들어온 메쉬 렌더
	void MeshRender();
	void OpacityMeshRender();
	void ShadowRender();

	//환경맵 세팅
	void SetEnvironment(std::string filename);

	void Update();

	void RenderText() const;
	void RenderSprite() const;
	void MakeModelEmpty();

	void RenderDebugDraw();

	//머테리얼 별로 소팅
	void RenderQueueSort();


	void RenderBegin();

	void GameAppRender();
	void EditorRender();


	void RenderEnvironment();

	void FinalRender();


	void RenderEnd();
	bool InitImgui(HWND hWnd);
	void RenderImgui();
	void UnInitImgui();

	// minjeong : Create Shadow VS & PS
	void CreateShadowVS();
	void CreateShadowPS();private:
		std::string BasePath = "../Resource/";
	const wchar_t* m_fontFilePath = L"../Resource/font/myfile.spritefont";
	vector<TextInformation> m_texts;
	vector<SpriteInformation> m_sprites;
	vector<DynamicTextInformation> m_dynamicTexts;
};