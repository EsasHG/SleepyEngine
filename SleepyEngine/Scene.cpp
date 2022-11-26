#include "Scene.h"
#include <glm/glm.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


#include "Entity.h"
#include "Components/TransformComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "ModelLibrary.h"
#include "Components/MeshComponent.h"
#include "Model.h"
#include "TransformSystem.h"
Scene::Scene()
{
	
	Entity* ent = CreateEntity("quad");

	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)));

	ModelLibrary::GetInstance().AddMesh("quad", vertices);
	ent->AddComponent<MeshComponent>("quad","default", "color");

	TransformSystem::SetPosition(ent, glm::vec3(-1.0f, 0.0f, 0.0f));
	TransformSystem::SetScale(ent, glm::vec3(0.8f, 0.8f, 0.8f));

	//Entity* guitarBackpack = CreateEntity();
	//guitarBackpack->AddComponent<MeshComponent>("Assets/backpack/backpack.obj", "default", "texture");

	//guitar = new Model("Assets/backpack/backpack.obj");
	std::vector<std::string> planetMeshes = ModelLibrary::GetInstance().AddMesh("Assets/planet/planet.obj");
	std::vector<Entity*> planetEntities;
	for (std::string mesh : planetMeshes)
	{
		Entity* e = CreateEntity(mesh);
		e->AddComponent<MeshComponent>(mesh);
		planetEntities.push_back(e);
		TransformSystem::SetPosition(e, glm::vec3(-4.0f, 6.0f, 0.0f));
	}

	std::vector<std::string> guitarMeshes = ModelLibrary::GetInstance().AddMesh("Assets/backpack/backpack.obj");
	std::vector<Entity*> guitarEntities;
	for (std::string mesh : guitarMeshes)
	{
		Entity* e = CreateEntity(mesh);
		e->AddComponent<MeshComponent>(mesh);
		if (guitarEntities.size()>0)
		{
			TransformSystem::SetParent(guitarEntities[0], e);
		}
		guitarEntities.push_back(e);

//		TransformSystem::SetScale(e, glm::vec3(0.5f, 0.5f, 0.5f));
//		TransformSystem::SetRotation(e, 90.0f, 0.0f, -45.0f);
//		TransformSystem::SetPosition(e, glm::vec3(2.0f, 0.0f, 0.0f));
	}
	TransformSystem::SetPosition(ent, glm::vec3(0.5f, -0.5f, 0.0f));
	TransformSystem::SetRotation(ent, glm::vec3(45.0f, 0.0f, 0.0f));

	//TransformSystem::SetPosition(guitarEntities[1], glm::vec3(1.0f));
	//TransformSystem::SetScale(guitarEntities[1], glm::vec3(1.3f));
	TransformSystem::Unparent(guitarEntities[1]);
	//planet = new Model("Assets/planet/planet.obj");
	//rock = new Model("Assets/rock/rock.obj");
	//boat = new Model("Assets/boat.fbx");
}

Entity* Scene::CreateEntity(std::string entityName)
{
	if (!m_SceneEntity)
	{
		m_SceneEntity = new Entity("Scene", this);
		m_SceneEntity->AddComponent<TransformComponent>();
		m_SceneEntity->GetComponent<TransformComponent>()->m_Name = "Scene";
	}
	Entity* entity  = new Entity(entityName, this);
	entity->AddComponent<TransformComponent>();
	entity->GetComponent<TransformComponent>()->m_Name = entityName;
	TransformSystem::SetParent(m_SceneEntity, entity);

	return entity;
}

void Scene::Update()
{
	//draw objects
	auto regView = m_registry.view<TransformComponent, MeshComponent>();
	for (auto [entity, transform, mesh] : regView.each())
	{
		Renderer::DrawMesh(mesh, transform);
	}
}

