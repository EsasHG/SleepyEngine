#include "SandboxScene.h"
#include <Entity.h>
#include <Components/LightComponent.h>
#include <Components/MeshComponent.h>
#include <TransformSystem.h>
#include <Mesh.h>
#include "Boid.h"


SandboxScene::SandboxScene()
{
	Sleepy::Entity& dirLight = CreateEntity("Directional Light");
	dirLight.AddComponent<Sleepy::DirLightComponent>(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(-0.2f, -0.6f, -0.3f));

	Sleepy::Entity& pointLight = CreateEntity("Point Light");
	pointLight.AddComponent<Sleepy::PointLightComponent>(glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.4f, 0.4f, 0.4f), 1.0f, 0.09f, 0.032f);

	Sleepy::Entity& ent = CreateGameObject<Sleepy::Entity>("quad");

	std::vector<Sleepy::Vertex> vertices;
	vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)));

	Sleepy::ModelLibrary::GetInstance().AddMesh("quad", vertices);
	ent.AddComponent<Sleepy::MeshComponent>("quad", "default", "color");

	Sleepy::TransformSystem::SetPosition(ent, glm::vec3(0.5f, -0.5f, 0.0f));
	Sleepy::TransformSystem::SetRotation(ent, glm::vec3(45.0f, 0.0f, 0.0f));
	Sleepy::TransformSystem::SetScale(ent, glm::vec3(0.8f, 0.8f, 0.8f));

	Sleepy::MeshGroup* guitarGroup = Sleepy::ModelLibrary::GetInstance().AddMesh("Assets\\backpack\\backpack.obj");
	Sleepy::MeshGroup* planetGroup = Sleepy::ModelLibrary::GetInstance().AddMesh("Assets\\planet\\planet.obj");

	if (guitarGroup)
	{
		Sleepy::Entity& guitarEntity = LoadMeshGroup(guitarGroup, *m_SceneEntity, "Backpack");
		Sleepy::Entity& guitarEntity2 = LoadMeshGroup(guitarGroup, *m_SceneEntity, "Backpack2");
		Sleepy::TransformSystem::SetPosition(guitarEntity2, glm::vec3(3.0f, 0.0f, 0.0f));
	}

	if (planetGroup)
	{
		Sleepy::Entity& planetEntity = LoadMeshGroup(planetGroup, *m_SceneEntity, "Planet");
		Sleepy::TransformSystem::SetPosition(planetEntity, glm::vec3(-4.0f, 6.0f, 0.0f));
		Sleepy::Entity& planetEntity2 = LoadMeshGroup(planetGroup, *m_SceneEntity, "Planet2");
		Sleepy::TransformSystem::SetPosition(planetEntity2, glm::vec3(4.0f, 6.0f, 0.0f));
	}

	boid = &CreateGameObject<Boid>("boid");
}

void SandboxScene::Init()
{

}

void SandboxScene::Update(double deltaTime)
{
	Scene::Update(deltaTime);
}
