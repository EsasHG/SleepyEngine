#pragma once
#include <entt/entt.hpp>
#include <string>
class Entity;
struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

class Scene
{
public:
	Scene();

	void Update();

	Entity* m_SceneEntity;
private:
	Entity* CreateEntity(std::string entityName);


	entt::registry m_registry;

	friend class Entity;
	friend class Application;

	//temp
	class Model* guitar;
	class Model* planet;
	class Model* rock;
	class Model* boat;
};

