#pragma once
#include "../Engine/Engine.h"

class GameApp : public Engine
{
public:
	GameApp(HINSTANCE hInstance);
	virtual ~GameApp() override;

	bool Initialize(UINT Width, UINT Height) override;

	ECS::World* DeserializeGame(const std::string filename);

	template<typename ComponentType>
	void AssignComponents(ECS::Entity* entity, json& componentData);

	void SetParent(ECS::Entity* child, ECS::Entity* parent);

	void SetParentTransform(ECS::Entity* child, ECS::Entity* parent);

	virtual void Update() override;
	virtual void Render() override;

private:
	std::string basePath = "../Resource/";
	ECS::World* m_IntroWorld;
	ECS::World* m_GameWorld;
	ECS::World* m_OutroWorld;
};

template<typename ComponentType>
inline void GameApp::AssignComponents(ECS::Entity* entity, json& componentData)
{
	if constexpr (std::is_base_of_v<Script, ComponentType>)
	{
		entity->Assign<ComponentType>(entity);
	}
	else
	{
		entity->Assign<ComponentType>();
		auto& component = entity->get<ComponentType>().get();

		component = componentData;
	}
}
