#pragma once
#include <vector>
class GameObject;

class World
{
public:
	World();
	~World();

private:
	std::vector<std::shared_ptr<GameObject>> m_pGameObjects;

public:
	void Enter();
	void Exit();
	void Update() const;
};


