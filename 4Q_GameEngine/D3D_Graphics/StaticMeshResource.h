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

	VertexShader m_vertexShader;		//어떤 버텍스 셰이더 쓸지 가지고 있음
	InputLayout m_inputLayout;			//인풋 레이아웃 설정

	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	UINT m_vertexBufferStride = 0;		// 버텍스 하나의 크기
	UINT m_vertexBufferOffset = 0;		// 버텍스 버퍼의 오프셋
	UINT m_vertexCount = 0;				// 버텍스 개수 
	UINT m_indexCount = 0;				// 인덱스 개수
	UINT m_materialIndex = 0;			// 머테리얼 인덱스

	vector<Vertex> m_vertices;
	vector<UINT> m_indices;

	
private:
	void CreateVertexBuffer(ID3D11Device* device, const vector<Vertex>& vertices, UINT vertexCount);
	void CreateIndexBuffer(ID3D11Device* device, const vector<UINT>& indices, UINT indexCount);

public:
	void Create(ID3D11Device* device, aiMesh* mesh);
	void CreateEnvironment(ID3D11Device* device, aiMesh* mesh);
	void Setting(wstring vs_name);

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
	Math::Vector3 m_BoundingBoxMin;	// 회전을 고려한 느슨한AABB
	Math::Vector3 m_BoundingBoxMax;	// 회전을 고려한 느슨한AABB

public:
	void Create(const std::string& path);
	void CreateEnvironment(const std::string& path);
	Material* GetMeshMaterial(UINT index);

	void GetAABB(DirectX::XMFLOAT3& center, DirectX::XMFLOAT3& Extents);
	void GetBoundingBox(DirectX::XMFLOAT3& center, DirectX::XMFLOAT3& Extents);
};