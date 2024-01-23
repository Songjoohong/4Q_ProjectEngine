#include "pch.h"
#include "StaticMeshResource.h"
#include "VertexShader.h"
#include "Material.h"
#include "D3D_Graphics.h"

void StaticSceneResource::Create(const std::string& path)
{
	// ���� ��ηκ��� FBX ���� �̸� ����
	size_t lastSlash = path.find_last_of('/');
	size_t lastDot = path.find_last_of('.');

	if (lastSlash != std::string::npos && lastDot != std::string::npos)
	{
		std::wstring finalFilePath(path.begin(), path.end());
		m_fileName = finalFilePath.substr(lastSlash + 1, lastDot - lastSlash - 1);
	}

	// FBX ���� ��θ� scene�� ���ε�
	Assimp::Importer importer;
	unsigned int importFlags = aiProcess_Triangulate |      // �ﰢ������ ��ȯ
		aiProcess_GenNormals |                              // �븻 ����
		aiProcess_GenUVCoords |                             // UV ����
		aiProcess_CalcTangentSpace |                        // ź��Ʈ ����
		aiProcess_LimitBoneWeights |                        // ���� ������ �޴� ������ �ִ� ������ 4���� ����
		aiProcess_ConvertToLeftHanded;                      // �޼� ��ǥ��� ��ȯ
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);

	const aiScene* scene = importer.ReadFile(path, importFlags);

	// Mesh, Material ���� Create
	m_meshes.resize(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		m_meshes[i].Create(Renderer::Instance->m_pDevice.Get(), scene->mMeshes[i]);
	}

	m_materials.resize(scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		m_materials[i].SetFileName(m_fileName);
		m_materials[i].Create(scene->mMaterials[i]);
	}

	importer.FreeScene();
	
}

void StaticMeshResource::Create(ID3D11Device* device, aiMesh* mesh)
{
	m_materialIndex = mesh->mMaterialIndex;

	m_vertices.resize(mesh->mNumVertices);
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		m_vertices[i].Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		m_vertices[i].Texcoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		m_vertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		m_vertices[i].Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
	}
	CreateVertexBuffer(device, m_vertices, mesh->mNumVertices);

	m_indices.resize(mesh->mNumFaces * 3);
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		m_indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		m_indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		m_indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}
	CreateIndexBuffer(device, m_indices, mesh->mNumFaces * 3);

	Setting();
}

void StaticMeshResource::CreateVertexBuffer(ID3D11Device* device, const vector<Vertex>& vertices, UINT vertexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	// ���ؽ� ���� ����
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices.data();
	HR_T(device->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

	// ���ؽ� ���� ����
	m_vertexCount = vertexCount;
	m_vertexBufferStride = sizeof(Vertex);
	m_vertexBufferOffset = 0;
}

void StaticMeshResource::CreateIndexBuffer(ID3D11Device* device, const vector<UINT>& indices, UINT indexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(UINT) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	// �ε��� ���� ����
	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices.data();
	device->CreateBuffer(&bd, &ibData, &m_pIndexBuffer);

	// �ε��� ���� ����
	m_indexCount = indexCount;
}

void StaticMeshResource::Setting()
{
	m_vertexShader.SetShader(L"VertexShader");
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_inputLayout.CreateInputLayout(layout,m_vertexShader.GetShader(),ARRAYSIZE(layout));
	
}

Material* StaticSceneResource::GetMeshMaterial(UINT index)
{
	assert(index<m_meshes.size());
	UINT matIndex = m_meshes[index].m_materialIndex;
	assert(matIndex < m_materials.size());
	return &m_materials[matIndex];
}