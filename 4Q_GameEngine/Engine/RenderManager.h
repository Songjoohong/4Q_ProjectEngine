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

	void SetBasePath(std::string filePath);

	void SetCamera(const DirectX::SimpleMath::Matrix matrix);
	
	void AddDebug(int entID, const std::string& text, const Vector3D& pos);
	void AddSprite(int entID, const std::string& filePath, MyPoint pos, float layer);

	void EditDebug(int entID, const std::string& text, const Vector3D& pos);
	void EditSprite(int entID, bool isRendered);

	Renderer* GetRender() { return m_Renderer; }
	void DeleteDebug(int entID);
	void DeleteSprite(int entID);

private:
	Renderer* m_Renderer;
	ResourceManager* m_ResourceManager;
};

