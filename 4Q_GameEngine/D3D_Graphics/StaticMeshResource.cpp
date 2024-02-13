#include "pch.h"
#include "StaticMeshResource.h"
#include "VertexShader.h"
#include "Material.h"
#include "D3D_Graphics.h"

void StaticSceneResource::Create(const std::string& path)
{
	// 파일 경로로부터 FBX 파일 이름 추출
	size_t lastSlash = path.find_last_of('/');
	size_t lastDot = path.find_last_of('.');

	if (lastSlash != std::string::npos && lastDot != std::string::npos)
	{
		std::wstring finalFilePath(path.begin(), path.end());
		m_fileName = finalFilePath.substr(lastSlash + 1, lastDot - lastSlash - 1);
	}
	
	// FBX 파일 경로를 scene에 바인딩
	Assimp::Importer importer;
	unsigned int importFlags = aiProcess_Triangulate |      // 삼각형으로 변환
		aiProcess_GenNormals |                              // 노말 생성
		aiProcess_GenUVCoords |                             // UV 생성
		aiProcess_CalcTangentSpace |                        // 탄젠트 생성
		aiProcess_GenBoundingBoxes |						// 바운딩 박스 생성
		aiProcess_LimitBoneWeights |                        // 본의 영향을 받는 정점의 최대 개수를 4개로 제한
		aiProcess_ConvertToLeftHanded;                      // 왼손 좌표계로 변환
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);

	const aiScene* scene = importer.ReadFile(path, importFlags);
	Math::Matrix localTM = Math::Matrix(&scene->mRootNode->mTransformation.a1).Transpose();

	aiNode* a=*scene->mRootNode->mChildren;
	// Mesh, Material 정보 Create
	m_meshes.resize(scene->mNumMeshes);
	CreateFromNode(scene->mRootNode,scene);
	scene->mRootNode->mTransformation;
	/*for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		m_meshes[i].Create(Renderer::Instance->m_pDevice.Get(), scene->mMeshes[i]);
	}*/

	m_materials.resize(scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		m_materials[i].SetFileName(m_fileName);
		m_materials[i].Create(scene->mMaterials[i]);
	}
	m_AABBmin = Math::Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_AABBmax = Math::Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	
	float absMax = 0.0f;
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* pMesh = scene->mMeshes[i];
		Math::Vector3 meshMin = Math::Vector3(pMesh->mAABB.mMin.x, pMesh->mAABB.mMin.y, pMesh->mAABB.mMin.z);
		Math::Vector3 meshMax = Math::Vector3(pMesh->mAABB.mMax.x, pMesh->mAABB.mMax.y, pMesh->mAABB.mMax.z);

		m_AABBmin = Math::Vector3::Min(m_AABBmin, meshMin);
		m_AABBmax = Math::Vector3::Max(m_AABBmax, meshMax);
	}
	absMax = max(m_AABBmax.Length(), m_AABBmin.Length());
	m_BoundingBoxMin = Math::Vector3(-absMax, -absMax, -absMax);
	m_BoundingBoxMax = Math::Vector3(absMax, absMax, absMax);

	importer.FreeScene();
	
}

void StaticSceneResource::CreateFromNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		m_meshes[node->mMeshes[i]].Create(Renderer::Instance->m_pDevice.Get(), scene->mMeshes[node->mMeshes[i]]);
		m_meshes[node->mMeshes[i]].m_localMatrix = Math::Matrix(&node->mTransformation.a1).Transpose();
		m_meshes[node->mMeshes[i]].m_meshName = node->mName.C_Str();
	}
	for (int j = 0; j < node->mNumChildren; j++)
	{
		CreateFromNode(node->mChildren[j], scene);
	}
	
}

void StaticSceneResource::CreateEnvironment(const std::string& path)
{
	// 파일 경로로부터 FBX 파일 이름 추출
	size_t lastSlash = path.find_last_of('/');
	size_t lastDot = path.find_last_of('.');

	if (lastSlash != std::string::npos && lastDot != std::string::npos)
	{
		std::wstring finalFilePath(path.begin(), path.end());
		m_fileName = finalFilePath.substr(lastSlash + 1, lastDot - lastSlash - 1);
	}

	// FBX 파일 경로를 scene에 바인딩
	Assimp::Importer importer;
	unsigned int importFlags = aiProcess_Triangulate |      // 삼각형으로 변환
		aiProcess_GenNormals |                              // 노말 생성
		aiProcess_GenUVCoords |                             // UV 생성
		aiProcess_CalcTangentSpace |                        // 탄젠트 생성
		aiProcess_LimitBoneWeights |                        // 본의 영향을 받는 정점의 최대 개수를 4개로 제한
		aiProcess_ConvertToLeftHanded;                      // 왼손 좌표계로 변환
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);

	const aiScene* scene = importer.ReadFile(path, importFlags);

	// Mesh, Material 정보 Create
	m_meshes.resize(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		m_meshes[i].CreateEnvironment(Renderer::Instance->m_pDevice.Get(), scene->mMeshes[i]);
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
		if(mesh->mTextureCoords[0])
			m_vertices[i].Texcoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		if (mesh->mNormals)
			m_vertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTangents)
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

	Setting(L"VertexShader");
}

void StaticMeshResource::CreateEnvironment(ID3D11Device* device, aiMesh* mesh)
{
	m_materialIndex = mesh->mMaterialIndex;

	m_vertices.resize(mesh->mNumVertices);
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		m_vertices[i].Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		if (mesh->mTextureCoords[0])
			m_vertices[i].Texcoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		if (mesh->mNormals)
			m_vertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTangents)
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

	Setting(L"VS_Environment");
}

void StaticMeshResource::CreateVertexBuffer(ID3D11Device* device, const vector<Vertex>& vertices, UINT vertexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	// 버텍스 버퍼 생성
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices.data();
	HR_T(device->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

	// 버텍스 버퍼 정보
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

	// 인덱스 버퍼 생성
	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices.data();
	device->CreateBuffer(&bd, &ibData, &m_pIndexBuffer);

	// 인덱스 버퍼 정보
	m_indexCount = indexCount;
}

void StaticMeshResource::Setting(wstring vs_name)
{
	m_vertexShader.SetShader(vs_name);
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

void StaticSceneResource::GetAABB(DirectX::XMFLOAT3& center, DirectX::XMFLOAT3& Extents)
{
	center = (m_AABBmin + m_AABBmax) * 0.5f;
	Extents = (m_AABBmax - m_AABBmin) * 0.5f;
}

void StaticSceneResource::GetBoundingBox(DirectX::XMFLOAT3& center, DirectX::XMFLOAT3& Extents)
{
	center = (m_BoundingBoxMin + m_BoundingBoxMax) * 0.5f;
	Extents = (m_BoundingBoxMax - m_BoundingBoxMin) * 0.5f;
}
