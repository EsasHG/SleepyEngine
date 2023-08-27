#include "SandboxScene.h"
#include <iostream>
#include <Entity.h>
#include <Components/LightComponent.h>
#include <Components/MeshComponent.h>
#include <TransformSystem.h>
#include <Mesh.h>

#include "Player.h"
#include "BoidManager.h"
SandboxScene::SandboxScene()
{
	std::vector<Sleepy::Vertex> vertices;
	vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
	vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)));

	Sleepy::Material& mat = Sleepy::ModelLibrary::GetInstance().CreateMaterial("color");
	mat.shininess = 2.0f;

	Sleepy::ModelLibrary::GetInstance().AddMesh("quad", vertices);

	Sleepy::Entity& dirLight = CreateEntity("Directional Light");
	dirLight.AddComponent<Sleepy::DirLightComponent>(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(-0.2f, -0.6f, -0.3f));

	Sleepy::Entity& pointLight = CreateEntity("Point Light");
	pointLight.AddComponent<Sleepy::PointLightComponent>(glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f), 
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	pointLight.SetPosition(glm::vec3(0.0f, 0.0f, 0.5f));

	Sleepy::MeshGroup* guitarGroup = Sleepy::ModelLibrary::GetInstance().AddMesh("Assets\\backpack\\backpack.obj");
	Sleepy::MeshGroup* planetGroup = Sleepy::ModelLibrary::GetInstance().AddMesh("Assets\\planet\\planet.obj");

	if (guitarGroup)
	{
		Sleepy::Entity& guitarEntity = LoadMeshGroup(guitarGroup, *m_SceneEntity, "Backpack");
		guitarEntity.SetPosition(glm::vec3(-5.0f, 0.0f, 10.0f));
		Sleepy::Entity& guitarEntity2 = LoadMeshGroup(guitarGroup, *m_SceneEntity, "Backpack2");
		guitarEntity2.SetPosition(glm::vec3(5.0f, 0.0f, 10.0f));
	}

	if (planetGroup)
	{
		Sleepy::Entity& planetEntity = LoadMeshGroup(planetGroup, *m_SceneEntity, "Planet");
		planetEntity.SetPosition(glm::vec3(-10.0f, 6.0f, -10.0f));
		controlledEntity = &planetEntity;
		Sleepy::Entity& planetEntity2 = LoadMeshGroup(planetGroup, *m_SceneEntity, "Planet2");
		planetEntity2.SetPosition(glm::vec3(10.0f, 6.0f, -10.0f));
	}
	
	CreateGameObject<BoidManager>("Boid Manager");

	std::vector<std::string> faces
	{
		"Assets\\skybox\\right.jpg",
		"Assets\\skybox\\left.jpg",
		"Assets\\skybox\\top.jpg",
		"Assets\\skybox\\bottom.jpg",
		"Assets\\skybox\\front.jpg",
		"Assets\\skybox\\back.jpg"
	};
	std::string face = "Assets\\skybox2.png";

	Sleepy::ModelLibrary::GetInstance().LoadCubemapTexture(faces);

}



void SandboxScene::BeginPlay()
{
	Sleepy::Scene::BeginPlay();

	CreateGameObject<Player>("Player");
}

void SandboxScene::Update(double deltaTime)
{
	Sleepy::Scene::Update(deltaTime);

}


//void SandboxScene::MoveDown()
//{
//	position -= up * frameCameraSpeed;
//
//}
//
//void SandboxScene::MoveUp()
//{
//	position += up * frameCameraSpeed;
//}
