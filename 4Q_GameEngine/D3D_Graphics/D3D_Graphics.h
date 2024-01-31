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
};

struct cbProjection
{
	Math::Matrix mProjcetion;
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
	ComPtr<ID3D11SamplerState> m_pSampler = nullptr;				//샘플러

	ComPtr<ID3D11Buffer> m_pWorldBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pViewBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pProjectionBuffer = nullptr;
	
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

	DirectX::BoundingFrustum m_frustumCmaera;


	
public:
	//d3d객체 초기화
	bool Initialize(HWND* Hwnd, UINT Width, UINT Height);


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
	
	//모델 만들어서 모델 리스트에 추가
	void CreateModel(string filename);

	void FrustumCulling(StaticModel* model);

	void SetCamera(Math::Vector3 position={0,0,-100},Math::Vector3 eye={0,0,1},Math::Vector3 up = {0,1,0});

	void ApplyMaterial(Material* pMaterial);

	//메쉬 렌더큐에 들어온 메쉬 렌더
	void MeshRender();

	void MakeModelEmpty();
	

	void RenderBegin();
	void Render();
	void RenderEnd();
private:
	string BasePath = "../Resource/";
};