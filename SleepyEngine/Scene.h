#pragma once
#include <entt/entt.hpp>
#include <string>
#include <unordered_map>
#include <memory>

namespace Sleepy
{

	class Entity;
	struct aiNode;
	struct aiMesh;
	struct aiScene;
	struct aiMaterial;
	struct MeshGroup;

	class Scene
	{
	public:
		Scene();
		~Scene();
		virtual void Init() = 0;
		virtual void Update() = 0;
		void DeleteEntity(Entity& entity);
		void DeleteAllComponents(Entity& entity);
		void Draw();

		/// <summary>
		/// Deletes all entities marked for deletion
		/// </summary>
		void CleanupRegistry();
		static void MarkForDeletion(Entity& entity);

		template<typename T>
		void RemoveComponent(Entity* entity);

		Entity* m_SceneEntity;
	protected:
		Entity& CreateEntity(std::string entityName);
		Entity& LoadMeshGroup(MeshGroup* meshGroup, Entity& parent, std::string groupName);
	private:

		entt::registry m_registry;
		std::vector<Entity*> entities;

		friend class Entity;
		friend class Application;
	};
}