#pragma once
#include "pch.h"


class StaticMeshResource;
class StaticModel;
class Material;

const size_t BUFFER_SIZE = 2;

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
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI���丮
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// ����ī�� ������ ���� ������ �������̽�
	ComPtr<ID3D11Device> m_pDevice = nullptr;						//����̽�
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;			//����̽����ؽ�Ʈ
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;					//����ü��
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;	//���� Ÿ�� ��
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;	//���� ���ٽ� ��
	ComPtr<ID3D11SamplerState> m_pSampler = nullptr;				//���÷�

	ComPtr<ID3D11Buffer> m_pViewBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pProjectionBuffer = nullptr;
	
	list<StaticModel*> m_pStaticModels;			//������ �� ����ƽ �� ����Ʈ

	//ī�޶� ���
	Math::Vector3 m_cameraPos, m_cameraEye, m_cameraUp;
	Math::Matrix m_viewMatrix;
	cbView m_viewMatrixCB;

	//�������� ���
	Math::Matrix m_projectionMatrix;
	cbProjection m_projectionMatrixCB;
public:
	//d3d��ü �ʱ�ȭ
	bool Initialize(HWND* Hwnd, UINT Width, UINT Height);


	//ȭ�� Ŭ����
	void Clear(float r=0,float g=0,float b=0);
	void Clear(Math::Vector3 color);

	//���ҽ� ��� ���� �� ����
	void SetPath(string filePath) { BasePath = filePath; }
	string GetPath() { return BasePath; }

	//�� ���� �� ����Ʈ�� �߰�
	StaticModel* LoadStaticModel(string filename);
	void AddStaticModel(string filename);

	void SetCamera(Math::Vector3 position={0,0,-100},Math::Vector3 eye={0,0,1},Math::Vector3 up = {0,1,0});

	void ApplyMaterial(Material* pMaterial);

	void StaticModelRender();
	void Render();
	void RenderEnd();
private:
	string BasePath = "../Resource/";
};