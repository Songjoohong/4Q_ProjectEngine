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
	void SetContext(ECS::World* world);

	void DragDropContentsBrowser(ECS::Entity* entity, std::filesystem::path file);
private:
	std::filesystem::path m_CurrentDirectory;
	ECS::World* m_World;

	ID3D11ShaderResourceView* texture;
};

