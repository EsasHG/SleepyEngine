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


class Deletion : Component
{
public:
	Deletion(Entity* entity, int d)
	{
		m_Entity = entity;
		m_componentType = DELETION;

	}
	friend class Scene;
};

Scene::Scene()
{

	//can one entity have both components?
	Entity* dirLight = CreateEntity("Directional Light");
	dirLight->AddComponent<DirLightComponent>(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), 
		glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(-0.2f, -0.6f, -0.3f));

	Entity* pointLight = CreateEntity("Point Light");
	pointLight->AddComponent<PointLightComponent>(glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.4f, 0.4f, 0.4f), 1.0f, 0.09f, 0.032f);

	//Entity* thirdLight = CreateEntity("Test Light");
	////thirdLight->AddComponent<DirLightComponent>();
	//thirdLight->AddComponent<PointLightComponent>(glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.4f, 0.4f, 0.4f), 1.0f, 0.09f, 0.032f);
	//thirdLight->AddComponent<DirLightComponent>(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
	//	glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(-0.2f, -0.6f, -0.3f));

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



	MeshGroup* guitarGroup = ModelLibrary::GetInstance().AddMesh("Assets\\backpack\\backpack.obj");
	MeshGroup* planetGroup = ModelLibrary::GetInstance().AddMesh("Assets\\planet\\planet.obj");

	if (guitarGroup)
	{
		Entity* guitarEntity = LoadMeshGroup(guitarGroup, m_SceneEntity, "Backpack");
		Entity* guitarEntity2 = LoadMeshGroup(guitarGroup, m_SceneEntity,"Backpack2");
		TransformSystem::SetPosition(guitarEntity2, glm::vec3(3.0f, 0.0f, 0.0f));
	}

	if (planetGroup)
	{
		Entity* planetEntity = LoadMeshGroup(planetGroup, m_SceneEntity, "Planet");
		TransformSystem::SetPosition(planetEntity, glm::vec3(-4.0f, 6.0f, 0.0f));
		Entity* planetEntity2 = LoadMeshGroup(planetGroup, m_SceneEntity, "Planet2");
		TransformSystem::SetPosition(planetEntity2, glm::vec3(4.0f, 6.0f, 0.0f));
	}
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
		ent = CreateEntity(groupName);
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

}
void Scene::Draw()
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


void Scene::CleanupRegistry()
{
	auto regView = m_registry.view<Deletion>();
	for (auto [entity, del] : regView.each())
		DeleteEntity(del.m_Entity);
}

void Scene::MarkForDeletion(Entity* entity)
{
	entity->AddComponent<Deletion>(2);
	auto transform = entity->GetComponent<TransformComponent>();
	for (auto child : transform->m_children)
	{
		MarkForDeletion(child->m_Entity);
	}
}

void Scene::DeleteEntity(Entity* entity)
{

	DeleteAllComponents(entity);

	m_registry.destroy(entity->m_entityHandle);
	
	delete entity;
}

void Scene::DeleteAllComponents(Entity* entity)
{
	/*
	if(entity->Has<Deletion>())
		m_registry.remove<Deletion>(entity->m_entityHandle);

	if (entity->Has<PointLightComponent>())
		m_registry.remove<PointLightComponent>(entity->m_entityHandle);

	DirLightComponent* dirLight = entity->GetComponent<DirLightComponent>();
	if (dirLight)
	{
		m_registry.remove<DirLightComponent>(entity->m_entityHandle);
	}

	//does this return correctly?
	LightComponent* light = entity->GetComponent<LightComponent>();
	if (light)
	{
		m_registry.remove<LightComponent>(entity->m_entityHandle);
	}

	MeshComponent* mesh = entity->GetComponent<MeshComponent>();
	if (mesh)
	{
		m_registry.remove<MeshComponent>(entity->m_entityHandle);
	}

	*/

	TransformComponent* transform = entity->GetComponent<TransformComponent>();
	if (transform && transform->m_parent)
	{
		TransformComponent* parent = transform->m_parent;

		int foundIndex = -1;
		for (int i = 0; i < parent->m_children.size(); i++)
		{
			if (*transform == *parent->m_children[i])			//will just. not work sometimes?
			{
				foundIndex = i;
				break;
			}
		}
		if(foundIndex != -1)
			parent->m_children.erase(parent->m_children.begin()+foundIndex);
		
		auto it = std::find(parent->m_children.begin(), parent->m_children.end(), transform);
		if (it != parent->m_children.end())
		{
			parent->m_children.erase(it);
		}
	}
	//m_registry.remove<TransformComponent>(entity->m_entityHandle);


}

