#pragma once
#include <entt/entt.hpp>
#include <string>
#include "Scene.h"

class Entity
{
public:
	Entity(std::string entityName, Scene* scene) : m_Name(entityName), m_scene(scene)
	{
		m_entityHandle = m_scene->m_registry.create();
	}

	~Entity()
	{
		RemoveAsChild();
	}

	template<typename T, typename ... Args>
	T& AddComponent(Args&&... args)
	{
		return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Entity*>(this),std::forward<Args>(args)...);
	}

	template<typename T>
	const T& GetComponent() const 
	{
		return m_scene->m_registry.get<T>(m_entityHandle);
	}

	template<typename T>
	T& GetComponent()
	{
		return m_scene->m_registry.get<T>(m_entityHandle);
	}

	template<typename T>
	const bool Has() const
	{
		return m_scene->m_registry.all_of<T>(m_entityHandle);
	}

	template<typename T>
	void RemoveComponent() 
	{
		m_scene->m_registry.remove<T>(m_entityHandle);
	}

	bool operator == (Entity const& e) const 
	{
		return m_entityHandle == e.m_entityHandle;
	}

	bool operator == (const Entity* e) const
	{
		return m_entityHandle == e->m_entityHandle;
	}


	Entity& GetParent();
		
	std::vector<Entity*> GetChildren();

	void MoveChild(Entity& child, int newIndex);

	void SetParent(Entity& entity);
	void Unparent();
	bool HasParent();
	bool HasChildren();

	std::string m_Name;
private:

	void RemoveAsChild();
	Scene* m_scene;
	entt::entity m_entityHandle;

	friend class Scene;				//So we can get the m_entityHandle to delete entity
	friend class RelationshipComponent;
};

