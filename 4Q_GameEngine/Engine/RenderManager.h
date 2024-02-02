#pragma once
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

	void RenderBegin();
	void Render();
	void RenderEnd();

	void CreateModel(std::string filename);

	void AddStaticMesh(const std::string& fileName, Vector3D& pos, Vector3D& rot, Vector3D& scale) const;

	void SetBasePath(std::string filePath);

	void SetCameraPos(Vector3D pos, Vector3D rot);

	void SetCameraPos(Vector3D pos, Vector3D eye, Vector3D up);
	
	void AddDebug(int entID, const std::string& text, const Vector3D& pos);

	void EditDebug(int entID, const std::string& text, const Vector3D& pos);

	Renderer* GetRender() { return m_Renderer; }
private:
	Renderer* m_Renderer;
	ResourceManager* m_ResourceManager;
};

