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
Scene::Scene()
{
	Entity* ent = CreateEntity();

	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)));

	ModelLibrary::GetInstance().AddMesh("quad", vertices);
	ent->AddComponent<MeshComponent>("quad","default", "color");

	ent->GetComponent<TransformComponent>()->position = glm::vec3(-1.0f, 0.0f, 0.0f);
	ent->GetComponent<TransformComponent>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);

	Entity* guitarBackpack = CreateEntity();
	//guitarBackpack->AddComponent<MeshComponent>("Assets/backpack/backpack.obj", "default", "texture");


	//guitar = new Model("Assets/backpack/backpack.obj");
	std::vector<std::string> planetMeshes = ModelLibrary::GetInstance().AddMesh("Assets/planet/planet.obj");
	std::vector<Entity*> planetEntities;
	for (std::string mesh : planetMeshes)
	{
		Entity* e = CreateEntity();
		e->AddComponent<MeshComponent>(mesh);
		planetEntities.push_back(e);
		e->GetComponent<TransformComponent>()->position = glm::vec3(-4.0f, 6.0f, 0.0f);

	}

	std::vector<std::string> guitarMeshes = ModelLibrary::GetInstance().AddMesh("Assets/backpack/backpack.obj");
	std::vector<Entity*> guitarEntities;
	for (std::string mesh : guitarMeshes)
	{
		Entity* e = CreateEntity();
		e->AddComponent<MeshComponent>(mesh);
		guitarEntities.push_back(e);
		e->GetComponent<TransformComponent>()->position = glm::vec3(2.0f, 0.0f, 0.0f);
	}



	//planet = new Model("Assets/planet/planet.obj");
	//rock = new Model("Assets/rock/rock.obj");
	//boat = new Model("Assets/boat.fbx");
}

Entity* Scene::CreateEntity()
{
	Entity* entity  = new Entity(this);
	entity->AddComponent<TransformComponent>();
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


	//model = glm::mat4(1.0f);
	//Renderer::SetShaderUniformMat4(m_TextureShaderId, "model", model);
	//guitar->Draw(m_TextureShaderId);

	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
	//Renderer::SetShaderUniformMat4(m_TextureShaderId, "model", model);
	//planet->Draw(m_TextureShaderId);

	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 2.0f));
	//Renderer::SetShaderUniformMat4(m_TextureShaderId, "model", model);
	//rock->Draw(m_TextureShaderId);
}

