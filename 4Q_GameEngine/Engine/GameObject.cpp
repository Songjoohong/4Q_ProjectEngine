#include "pch.h"
#include "GameObject.h"

#include "Component.h"

GameObject::~GameObject()
{
	for(const auto& component : m_pComponents)
	{
		delete component;
	}
	m_pComponents.clear();
}



void GameObject::Update() const
{
	for(auto& component : m_pComponents)
	{
		component->Update();
	}
}
