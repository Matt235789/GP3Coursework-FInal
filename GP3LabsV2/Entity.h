#pragma once
#include "pch.h"
#include "Transform.h"
#include "Component.h"
#include "Log.h"


class Entity
{
private:
	std::vector<Component*> m_components;
	Transform* m_transform = nullptr;

public:
	Entity();

	void AddComponent(Component* c);
	template<class T> void AddComponent();
	template<class T> T* GetComponent();

	void OnUpdate(float deltaTime);
	void OnRender();
	void OnStart();

	Transform* GetTransform() { return m_transform; }

};

template<class T>
void Entity::AddComponent()
{
	T* t = new T();
	Component* c = dynamic_cast<Component*>(t);
	if (c != nullptr)
	{
		c->m_entity = this;
		m_components.push_back(c);
		for (auto& c : m_components)
		{
			c->OnStart();
		}
	}
	else
	{
		LOG_DEBUG("Can't add component, does not inherit from component", errType::ERROR);

	}
}

template<class T>
T* Entity::GetComponent()
{
	for (auto& c : m_components)
	{
		T* cd = dynamic_cast<T*>(c);
		if (cd != nullptr)
		{
			return cd;
		}
	}
	return nullptr;
}
