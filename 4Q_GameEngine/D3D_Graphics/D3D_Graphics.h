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

class Renderer
{
public:
	static Renderer* Instance;
	
	Renderer();
	~Renderer();
public:
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI���丮
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// ����ī�� ������ ���� ������ �������̽�
	ComPtr<ID3D11Device> m_pDevice = nullptr;						//����̽�
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;			//����̽����ؽ�Ʈ
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;					//����ü��
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;	//���� Ÿ�� ��
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;	//���� ���ٽ� ��
	ComPtr<ID3D11SamplerState> m_pSampler = nullptr;				//���÷�

	// shadow ���� ��ü
	ComPtr<ID3D11Texture2D> m_pShadowMap;
	ComPtr<ID3D11DepthStencilView> m_pShadowMapDSV;
	ComPtr<ID3D11ShaderResourceView> m_pShadowMapSRV;
	D3D11_VIEWPORT m_viewport;
	D3D11_VIEWPORT m_shadowViewport;

	ComPtr<ID3D11Buffer> m_pWorldBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pViewBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pProjectionBuffer = nullptr;
	
	vector<StaticModel*> m_pStaticModels;			//������ �� ����ƽ �� ����Ʈ

	list<StaticMeshInstance*>m_pMeshInstance;	//������ �� �޽� �ν��Ͻ� ����Ʈ

	//���� ���
	Math::Matrix m_worldMatrix;
	cbWorld m_worldMatrixCB;

	//ī�޶� ���
	Math::Vector3 m_cameraPos, m_cameraEye, m_cameraUp;
	Math::Matrix m_viewMatrix;
	cbView m_viewMatrixCB;

	//�������� ���
	Math::Matrix m_projectionMatrix;
	cbProjection m_projectionMatrixCB;
public:
	//d3d��ü �ʱ�ȭ
	bool Initialize(HWND* hWnd, UINT width, UINT height);


	//ȭ�� Ŭ����
	void Clear(float r=0,float g=0,float b=0);
	void Clear(Math::Vector3 color);

	//���ҽ� ��� ���� �� ����
	void SetPath(string filePath) { BasePath = filePath; }
	string GetPath() { return BasePath; }

	//�� �𵨿� ���� �Է�
	void AddStaticModel(string filename, Math::Vector3& pos, Math::Vector3& rot, Math::Vector3& scale);

	//�޽� �ν��Ͻ� ����ť�� �߰�
	void AddMeshInstance(StaticModel* model);
	
	//�� ���� �� ����Ʈ�� �߰�
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