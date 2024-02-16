#pragma once

class PrefabManager;

namespace ECS { class Entity; }
namespace ECS { class World; }

class ID3D11ShaderResourceView;

class ContentsBrowserPanel
{
public:
	ContentsBrowserPanel();
	void Initialize();
	void RenderImGui();
	void SetContext(ECS::World* world, std::shared_ptr<PrefabManager> prefabManager);

private:
	std::filesystem::path m_CurrentDirectory;
	ECS::World* m_World;
	std::shared_ptr<PrefabManager> m_PrefabManager;
	ID3D11ShaderResourceView* texture;
};

