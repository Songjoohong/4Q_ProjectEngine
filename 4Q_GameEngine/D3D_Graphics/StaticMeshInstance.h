#pragma once
class StaticMeshResource;
class Material;

struct cbWorld
{
	Math::Matrix worldTransform;
};

class StaticMeshInstance
{
public:
	StaticMeshInstance();
	~StaticMeshInstance();

public:
	StaticMeshResource* m_pMeshResource = nullptr;
	Material* m_pMaterial = nullptr;
	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh의 월드행렬을 가진 노드의 포인터

	cbWorld m_worldTransfromCB;
	ComPtr<ID3D11Buffer>m_worldBuffer;
public:
	void Create(StaticMeshResource* pMeshResource, Math::Matrix* pWorld, Material* pMaterial);
	virtual void Render(ID3D11DeviceContext* deviceContext);

	void Initialize();
};