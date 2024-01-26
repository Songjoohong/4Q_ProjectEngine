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

	vector<Vertex> m_vertices;
	vector<UINT> m_indices;

private:
	void CreateVertexBuffer(ID3D11Device* device, const vector<Vertex>& vertices, UINT vertexCount);
	void CreateIndexBuffer(ID3D11Device* device, const vector<UINT>& indices, UINT indexCount);

public:
	void Create(ID3D11Device* device, aiMesh* mesh);
	void Setting();

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

public:
	void Create(const std::string& path);
	Material* GetMeshMaterial(UINT index);
};