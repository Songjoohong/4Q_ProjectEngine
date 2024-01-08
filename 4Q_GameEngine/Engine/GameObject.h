#pragma once
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;
class Component;
class GameObject
{
public:
	GameObject() = default;
	~GameObject();

public:
	GameObject* m_ParentObject = nullptr;
	vector<Component*> m_pComponents;
	std::string m_Name = "NoName";
	int m_ComponentCode = 0;
public:
	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		const bool bIsComponent = std::is_base_of<Component, T>::value;
		assert(bIsComponent);
		std::shared_ptr<T> newComponent = new T();
		newComponent->SetOwner(this);
		newComponent->SetCode(this);
		m_pComponents.push_back(newComponent);
		return newComponent;
	}

	template<typename T>
	std::shared_ptr<T> FindComponent()
	{
		for(auto& it : m_pComponents)
		{
			if(dynamic_cast<T*>(it) != nullptr)
			{
				return it;
			}
		}
		return nullptr;
	}
	void Update() const;
};

