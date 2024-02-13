#pragma once
#include "VertexShader.h"
#include "InputLayout.h"
#include "Material.h"

class Material;
struct aiMesh;

struct Vertex
{
	Vector3 Position;
	Vector2 Texcoord;
	Vector3 Normal;
	Vector3 Tangent;
};

class StaticMeshResource
{
public:
	StaticMeshResource() {}
	~StaticMeshResource() {}

	VertexShader m_vertexShader;		//� ���ؽ� ���̴� ���� ������ ����
	InputLayout m_inputLayout;			//��ǲ ���̾ƿ� ����

	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	UINT m_vertexBufferStride = 0;		// ���ؽ� �ϳ��� ũ��
	UINT m_vertexBufferOffset = 0;		// ���ؽ� ������ ������
	UINT m_vertexCount = 0;				// ���ؽ� ���� 
	UINT m_indexCount = 0;				// �ε��� ����
	UINT m_materialIndex = 0;			// ���׸��� �ε���

	std::vector<Vertex> m_vertices;
	std::vector<UINT> m_indices;

	
private:
	void CreateVertexBuffer(ID3D11Device* device, const std::vector<Vertex>& vertices, UINT vertexCount);
	void CreateIndexBuffer(ID3D11Device* device, const std::vector<UINT>& indices, UINT indexCount);

public:
	void Create(ID3D11Device* device, aiMesh* mesh);
	void CreateEnvironment(ID3D11Device* device, aiMesh* mesh);
	void Setting(std::wstring vs_name);

};

class StaticSceneResource
{
public:
	StaticSceneResource(){}
	~StaticSceneResource(){}

public:
	std::wstring m_fileName;
	std::vector<StaticMeshResource>m_meshes;
	std::vector<Material> m_materials;

	Math::Matrix m_localMatrix;

	Math::Vector3 m_AABBmin;
	Math::Vector3 m_AABBmax;
	Math::Vector3 m_BoundingBoxMin;	// ȸ���� ����� ������AABB
	Math::Vector3 m_BoundingBoxMax;	// ȸ���� ����� ������AABB

public:
	void Create(const std::string& path);
	void CreateEnvironment(const std::string& path);
	Material* GetMeshMaterial(UINT index);

	void GetAABB(DirectX::XMFLOAT3& center, DirectX::XMFLOAT3& Extents);
	void GetBoundingBox(DirectX::XMFLOAT3& center, DirectX::XMFLOAT3& Extents);
};