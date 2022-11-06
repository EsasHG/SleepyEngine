#pragma once
#include <entt/entt.hpp>
#include <string>
#include "Scene.h"
#include "Components/TransformComponent.h"

class Entity
{
public:
	Entity(std::string entityName, Scene* scene);

	template<typename T, typename ... Args>
	T& AddComponent(Args&&... args)
	{
		return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	const T* GetComponent() const 
	{
		return m_scene->m_registry.try_get<T>(m_entityHandle);
	}

	template<typename T>
	T* GetComponent()
	{
		return m_scene->m_registry.try_get<T>(m_entityHandle);
	}
	std::string m_Name;
private:
	Scene* m_scene;
	entt::entity m_entityHandle;
};

