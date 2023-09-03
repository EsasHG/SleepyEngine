#pragma once
#include <entt/entt.hpp>
#include <string>
#include <glm/glm.hpp>
#include "SceneBase.h"

namespace Sleepy
{
	class Entity
	{
	public:
		Entity(std::string entityName, SceneBase* scene);
	
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
	
		virtual void BeginPlay() {};
		virtual void Update(double deltaTime) {};

		Entity& GetParent();
			
		std::vector<Entity*> GetChildren();
	
		void MoveChild(Entity& child, int newIndex);
	
		void SetParent(Entity& entity);
		void Unparent();
		bool HasParent();
		bool HasChildren();

		glm::vec3 GetPosition();
		glm::vec3 GetRotation();
		glm::vec3 GetScale();

		void SetPosition(glm::vec3 pos);
		void SetRotation(glm::vec3 rot);
		void SetScale(glm::vec3 scale);
		entt::entity GetHandle() { return m_entityHandle; }
		std::string m_Name;

		bool enableUpdate = false;
		bool bActive = false;

		class Scene* GetScene();
	private:
	
		void RemoveAsChild();
		SceneBase* m_scene;
		entt::entity m_entityHandle;
		class TransformComponent* transformComp;
		class RelationshipComponent* relationshipComp;
	
		friend class SceneBase;				//So we can get the m_entityHandle to delete entity
		friend class Scene;					//So we can get the m_entityHandle to add to entities in SceneBase from Scene
		friend class RelationshipComponent;
		friend class TransformSystem;
	};
}