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
	
	void AddStaticModel(std::string filename, Vector3D pos, Vector3D view);

	void Render();

private:
	Renderer* m_Renderer;
	ResourceManager* m_ResourceManager;
};

