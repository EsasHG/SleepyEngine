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
#include "Components/LightComponent.h"
#include "Model.h"
#include "TransformSystem.h"

Scene::Scene()
{
	Entity* dirLight = CreateEntity("Directional Light");
	dirLight->AddComponent<DirLightComponent>(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), 
		glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(-0.2f, -0.6f, -0.3f));

	Entity* pointLight = CreateEntity("Point Light");
	pointLight->AddComponent<PointLightComponent>(glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.4f, 0.4f, 0.4f), 1.0f, 0.09f, 0.032f);

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
	
	TransformSystem::SetPosition(ent, glm::vec3(0.5f, -0.5f, 0.0f));
	TransformSystem::SetRotation(ent, glm::vec3(45.0f, 0.0f, 0.0f));
	TransformSystem::SetScale(ent, glm::vec3(0.8f, 0.8f, 0.8f));

	//Entity* guitarBackpack = CreateEntity();
	//guitarBackpack->AddComponent<MeshComponent>("Assets/backpack/backpack.obj", "default", "texture");

	//guitar = new Model("Assets/backpack/backpack.obj");
	//std::vector<std::string> planetMeshes = ModelLibrary::GetInstance().AddMesh("Assets/planet/planet.obj");
	//std::vector<Entity*> planetEntities;
	//for (std::string mesh : planetMeshes)
	//{
	//	Entity* e = CreateEntity(mesh);
	//	e->AddComponent<MeshComponent>(mesh);
	//	planetEntities.push_back(e);
	//	TransformSystem::SetPosition(e, glm::vec3(-4.0f, 6.0f, 0.0f));
	//}


	MeshGroup* guitarGroup = ModelLibrary::GetInstance().AddMesh("Assets\\backpack\\backpack.obj");
	MeshGroup* planetGroup = ModelLibrary::GetInstance().AddMesh("Assets\\planet\\planet.obj");


	//something about the order here matters... dont know why

	if(guitarGroup)
		Entity* guitarEntity = LoadMeshGroup(guitarGroup, m_SceneEntity,"Backpack");

	if (planetGroup)
	{
		Entity* planetEntity = LoadMeshGroup(planetGroup, m_SceneEntity, "Planet");
		TransformSystem::SetPosition(planetEntity, glm::vec3(-4.0f, 6.0f, 0.0f));
	}
	
	//for (std::string mesh : guitarMeshes)
	//{
	//	Entity* e = CreateEntity(mesh);
	//	e->AddComponent<MeshComponent>(mesh);
	//	if (guitarEntities.size()>0)
	//	{
	//		TransformSystem::SetParent(guitarEntities[0], e);
	//	}
	//	guitarEntities.push_back(e);
	//
//	//	TransformSystem::SetScale(e, glm::vec3(0.5f, 0.5f, 0.5f));
//	//	TransformSystem::SetRotation(e, 90.0f, 0.0f, -45.0f);
//	//	TransformSystem::SetPosition(e, glm::vec3(2.0f, 0.0f, 0.0f));
	//}


	//TransformSystem::SetPosition(guitarEntities[1], glm::vec3(1.0f));
	//TransformSystem::SetScale(guitarEntities[1], glm::vec3(1.3f));
	//TransformSystem::Unparent(guitarEntities[1]);
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
		//Children c = m_SceneEntity->AddComponent<Children>();

		//c.first.GetComponent<Hierarchy>();

	}
	Entity* entity  = new Entity(entityName, this);
	entity->AddComponent<TransformComponent>();
	TransformSystem::SetParent(m_SceneEntity, entity);

	return entity;
}

//have not been able to test this with groups that have multiple children
Entity* Scene::LoadMeshGroup(MeshGroup* meshGroup, Entity* parent, std::string groupName)
{
	Entity* ent = nullptr; 
	MeshRef* currentChild = meshGroup->firstChild;
	MeshGroup* currentChildGroup = meshGroup->firstChildGroup;

	if (currentChild && !currentChild->nextChildMesh) // if only one child mesh, add mesh to this entity
	{
		ent = CreateEntity(currentChild->meshName);
		TransformSystem::SetParent(parent, ent);
		ent->AddComponent<MeshComponent>(currentChild->meshName);
	}
	else if(currentChild)
	{
		ent = CreateEntity(groupName);
		TransformSystem::SetParent(parent, ent);

		while (currentChild)
		{
			Entity* meshEnt = CreateEntity(currentChild->meshName);
			meshEnt->AddComponent<MeshComponent>(currentChild->meshName);
			TransformSystem::SetParent(ent, meshEnt);
			currentChild = currentChild->nextChildMesh;
		}
	}
	else
	{
		if (currentChildGroup && !currentChildGroup->nextGroup)
		{
			ent = LoadMeshGroup(currentChildGroup, parent, groupName + "_" + std::to_string(0));
			return ent;
		}
		else
			ent = CreateEntity(groupName);
	}

	int i = 0;
	while (currentChildGroup)
	{

		Entity* entity = LoadMeshGroup(currentChildGroup, ent, groupName+"_"+std::to_string(i));
		currentChildGroup = currentChildGroup->nextGroup;
		i++;
	}

	return ent;
}

void Scene::Update()
{
	//update light values. I will figure out something smarter for this later lmao
	unsigned int shaderID = ModelLibrary::GetInstance().GetShader("default");
	auto pointLightView = m_registry.view<TransformComponent, PointLightComponent>();
	for (auto [entity, transform, light] : pointLightView.each())
		Renderer::SetPointLightValues(shaderID, transform, light);

	auto dirLightView = m_registry.view<TransformComponent, DirLightComponent>();
	for (auto [entity, transform, light] : dirLightView.each())
		Renderer::SetDirLightValues(shaderID, transform, light);

	//draw objects
	auto regView = m_registry.view<TransformComponent, MeshComponent>();
	for (auto [entity, transform, mesh] : regView.each())
	{
		Renderer::DrawMesh(mesh, transform);
	}
}