#pragma once
#include <entt/entt.hpp>
#include <string>
#include <unordered_map>
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

	void Update();
	void DeleteEntity(Entity* entity);
	void DeleteAllComponents(Entity* entity);
	void Draw();

	/// <summary>
	/// Deletes all entities marked for deletion
	/// </summary>
	void CleanupRegistry();
	static void MarkForDeletion(Entity* entity);
	Entity* m_SceneEntity;
private:
	Entity* CreateEntity(std::string entityName);
	Entity* LoadMeshGroup(MeshGroup* meshGroup, Entity* parent, std::string groupName);

	entt::registry m_registry;

	friend class Entity;
	friend class Application;
};

