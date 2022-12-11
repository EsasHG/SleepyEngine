#pragma once
#include <entt/entt.hpp>
#include <string>
#include <unordered_map>
#include <memory>
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
	void Update();
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
private:
	Entity& CreateEntity(std::string entityName);
	Entity& LoadMeshGroup(MeshGroup* meshGroup, Entity& parent, std::string groupName);

	entt::registry m_registry;
	std::vector<Entity*> entities;

	friend class Entity;
	friend class Application;
};


