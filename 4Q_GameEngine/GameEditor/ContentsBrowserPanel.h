#pragma once

class PrefabManager;
class NameManager;
namespace ECS { class Entity; }
namespace ECS { class World; }

class ID3D11ShaderResourceView;

class ContentsBrowserPanel
{
public:
	ContentsBrowserPanel();
	void Initialize();
	void RenderImGui();
	void SetContext(ECS::World* world, std::shared_ptr<PrefabManager> prefabManager, std::shared_ptr<NameManager> NameManager);

private:
	std::filesystem::path m_CurrentDirectory;
	ECS::World* m_World;
	std::shared_ptr<PrefabManager> m_PrefabManager;
	std::shared_ptr<NameManager> m_NameManager;
	ID3D11ShaderResourceView* texture;
};

