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
	ComPtr<IDXGIFactory4> m_pDXGIFactory;		// DXGI���丮
	ComPtr<IDXGIAdapter3> m_pDXGIAdapter;		// ����ī�� ������ ���� ������ �������̽�
	ComPtr<ID3D11Device> m_pDevice = nullptr;						//����̽�
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;			//����̽����ؽ�Ʈ
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;					//����ü��
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;	//���� Ÿ�� ��
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;	//���� ���ٽ� ��
	ComPtr<ID3D11SamplerState> m_pSampler = nullptr;				//���÷�

	ComPtr<ID3D11Buffer> m_pWorldBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pViewBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_pProjectionBuffer = nullptr;
	
	vector<StaticModel*> m_pStaticModels;			//������ �� ����ƽ �� ����Ʈ

	list<StaticMeshInstance*>m_pMeshInstance;	//������ �� �޽� �ν��Ͻ� ����Ʈ

	//spritefont ������
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

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
	bool Initialize(HWND* Hwnd, UINT Width, UINT Height);


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

	//����� ���� �߰�
	void AddDebugInformation(int id, const std::string& text, const Vector3D& position);

	// ����� ���� ����
	void EditDebugInformation(int id, const std::string& text, const Vector3D& position);
	//�� ���� �� ����Ʈ�� �߰�
	void CreateModel(string filename);

	//���� ��ǥ ndc�� ��ȯ
	DirectX::XMFLOAT3 ConversionToNDC(const Vector3D& pos) const;


	StaticModel* LoadStaticModel(string filename);

	void SetCamera(Math::Vector3 position={300.f,100.f,-100},Math::Vector3 eye={0,0,1},Math::Vector3 up = {0,1,0});

	void ApplyMaterial(Material* pMaterial);

	void MeshRender();
	void RenderText() const;
	

	void RenderBegin();
	void Render();
	void RenderEnd();
private:
	string BasePath = "../Resource/";
	const wchar_t* m_fontFilePath = L"../Resource/font/bitstream.spritefont";
	vector<DebugInformation> m_debugs;
};