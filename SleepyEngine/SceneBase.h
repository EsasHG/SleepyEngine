#pragma once
#include <entt/entt.hpp>
#include <string>
#include <unordered_map>
#include <memory>

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Sleepy
{

	class Entity;
	struct MeshGroup;

	class SceneBase 
	{
		public:
			SceneBase();
			~SceneBase();
			virtual void BeginPlay() = 0;
			virtual void Update(double deltaTime) = 0;
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
			Entity* GetEntity(entt::entity entityHandle);

			Entity* m_SceneEntity;
		protected:
			void CreateSceneEntity();
	
			Entity& CreateEntity(std::string entityName);
			Entity& LoadMeshGroup(MeshGroup* meshGroup, Entity& parent, std::string groupName);
	
			entt::registry m_registry;
			//TODO: Hate having both of these. At least while entities has everything in gameEntities already
			std::map<entt::entity, Entity*> entities;
			std::vector<Entity*> gameEntities;
			friend class Entity;
			friend class Application;
		private:
		
	};

}
