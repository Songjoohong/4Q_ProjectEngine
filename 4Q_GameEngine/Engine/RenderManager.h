#pragma once
#include <directxtk/SimpleMath.h>

#include "ISingleton.h"
#include "Vector3D.h"

class Renderer;
class ResourceManager;

class RenderManager : public ISingleton<RenderManager>
{
public:
	RenderManager() = default;
	~RenderManager() override = default;

public:
	void Initialize(HWND* hwnd,UINT width,UINT height);
	void UnInitialize();

	void Update();

	void RenderBegin();
	void Render();
	void RenderEnd();

	void CreateModel(std::string filename);

	void AddStaticMesh(const std::string& fileName, DirectX::SimpleMath::Matrix worldTM) const;

	void AddColliderBox(const Vector3D center, const Vector3D extents, const bool collision);

	void SetBasePath(std::string filePath);

	void SetCamera(const DirectX::SimpleMath::Matrix matrix);
	
	void AddText(int entID, const std::string& text, const Vector3D& pos);
	void AddSprite(int entID, const std::string& filePath, POINT pos, float layer);

	void EditText(int entID, const std::string& text, const Vector3D& pos);
	void EditSprite(int entID, bool isRendered);

	Renderer* GetRender() { return m_Renderer; }
	void DeleteText(int entID);
	void DeleteSprite(int entID);

private:
	Renderer* m_Renderer;
	ResourceManager* m_ResourceManager;
};

