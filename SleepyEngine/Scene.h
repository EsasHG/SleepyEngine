#pragma once
#include <entt/entt.hpp>
#include <string>
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

	void Draw();
	Entity* m_SceneEntity;
private:
	Entity* CreateEntity(std::string entityName);
	Entity* LoadMeshGroup(MeshGroup* meshGroup, Entity* parent, std::string groupName);

	entt::registry m_registry;

	friend class Entity;
	friend class Application;

	//temp
	class Model* guitar;
	class Model* planet;
	class Model* rock;
	class Model* boat;
};

