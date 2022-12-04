#pragma once
#include <entt/entt.hpp>
#include <string>
#include "Scene.h"
#include "Components/TransformComponent.h"

//struct Hierarchy {
//	Entity& parent;
//	Entity& nextChild;
//	Entity& prevChild;
//};
//
//struct Children
//{
//	Entity& first;
//};


class Entity
{
public:
	Entity(std::string entityName, Scene* scene) : m_Name(entityName), m_scene(scene)
	{
		m_entityHandle = m_scene->m_registry.create();
	}

	template<typename T, typename ... Args>
	T& AddComponent(Args&&... args)
	{
		return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Entity*>(this),std::forward<Args>(args)...);
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

	template<typename T>
	const bool Has() const
	{
		return m_scene->m_registry.all_of<T>(m_entityHandle);
	}

	template<typename T>
	void RemoveComponent() 
	{
		//return m_scene->MarkRemoveComponent<T>(this);
		m_scene->m_registry.remove<T>(m_entityHandle);
	}

	std::string m_Name;
private:
	Scene* m_scene;
	entt::entity m_entityHandle;

	friend class TransformSystem;	//so transformsystem can find m_scene and set that as parent when unparenting
	friend class Scene;				//So we can get the m_entityHandle to delete entity
};

