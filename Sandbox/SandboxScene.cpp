#include "SandboxScene.h"
#include <iostream>
#include <Entity.h>
#include <Components/LightComponent.h>
#include <Components/MeshComponent.h>
#include <TransformSystem.h>
#include <Mesh.h>
#include "ImGui/imgui.h"
struct gridCell
{
	int index;

	int minX;
	int maxX;
	int minY;
	int maxY;
	int minZ;
	int maxZ;
	std::vector<Boid*> boidsInCell;
};
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
		Sleepy::Entity& planetEntity2 = LoadMeshGroup(planetGroup, *m_SceneEntity, "Planet2");
		planetEntity2.SetPosition(glm::vec3(10.0f, 6.0f, -10.0f));
	}
}

static const int gridMin = -20, gridMax = 20;
static const int cellSize = 4;
static const int cellEnd = cellSize + gridMin;
static const int cellCountPerRow = (gridMax - gridMin) / cellSize;
static const int cellCountPerRow2 = cellCountPerRow * cellCountPerRow;
static const int cellCount = cellCountPerRow * cellCountPerRow2;

void SandboxScene::BeginPlay()
{
	Sleepy::Scene::BeginPlay();


	//draw quads in grid
	/*
	gridCell cellArr[cellCount];
	for (int x = 0; x < cellCountPerRow; x++)
	{
		for (int y = 0; y < cellCountPerRow; y++)
		{
			for (int z = 0; z < cellCountPerRow; z++)
			{
				Sleepy::Entity& e = CreateEntity("grid_" +std::to_string(x) + std::to_string(y) + std::to_string(z));
				e.AddComponent<Sleepy::MeshComponent>("quad", "color", "color");
				e.SetPosition(glm::vec3(x*cellSize + gridMin, y * cellSize + gridMin, z * cellSize + gridMin));
				int cellIndex = x * cellCountPerRow2 + y * cellCountPerRow + z;
				cellArr[cellIndex].index = cellIndex;
				cellArr[cellIndex].minX = x * cellEnd;
				cellArr[cellIndex].maxX = (x + 1) * cellEnd;

				cellArr[cellIndex].minY = y * cellEnd;
				cellArr[cellIndex].maxY = (y + 1) * cellEnd;

				cellArr[cellIndex].minZ = z * cellEnd;
				cellArr[cellIndex].maxZ = (z + 1) * cellEnd;
			}
		}
	}
	*/

	//spawn boids
	boids.clear();
	boids.reserve(sizeof(Boid*) * maxBoids);
	int boidsInGroup1 = 3;
	int boidsInGroup2 = 3;

	for (int i = 0; i < maxBoids; i++)
	{
		Boid& b = CreateGameObject<Boid>("boid", info);
		boids.push_back(&b);
		b.SetPosition(glm::vec3(((std::rand() % 2000) / 100.0f) - 10, ((std::rand() % 2000) / 100.0f) - 10, ((std::rand() % 2000) / 100.0f) - 10));
		b.SetScale(glm::vec3(0.05f));
		if (i < boidsInGroup1)
			b.inGroup1 = true;
		else if (i < boidsInGroup1 + boidsInGroup1)
			b.inGroup2 = true;
	}

}

void SandboxScene::Update(double deltaTime)
{
	Sleepy::Scene::Update(deltaTime);

	ImGui::Begin("Boid Manager");
	ImGui::DragFloat("MinDistance", &info.minDistance, 0.001f, 0.0f, 10.0f);
	ImGui::DragFloat("ViewRange", &info.viewRange, 0.01f, 0.0f, 50.0f);
	ImGui::DragFloat("AvoidFactor", &info.avoidFactor, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("MatchingFactor", &info.matchingFactor, 0.001f, 0.0f, 0.9999f);
	ImGui::DragFloat("CenteringFactor", &info.centeringFactor, 0.0001f, 0.0000f, 0.1000f,"%.4f");
	ImGui::DragFloat("TurnFactor", &info.turnFactor, 0.01f, 0.0f, 5.0f);
	ImGui::DragFloat("MaxSpeed", &info.maxSpeed, 0.001f, 0.0f, 50.0f);
	ImGui::DragFloat("MinSpeed", &info.minSpeed, 0.001f, 0.0f, 50.0f);
	ImGui::DragFloat("BiasVal", &info.biasVal, 0.000001f,0.0f, 1.0f, "%.6f");

	ImGui::End();


	//setup grid
	gridCell cellArr[cellCount];
	for (int x = 0; x < cellCountPerRow; x++)
	{
		for (int y = 0; y < cellCountPerRow; y++)
		{
			for (int z = 0; z < cellCountPerRow; z++)
			{
				int cellIndex = x * cellCountPerRow2 + y * cellCountPerRow + z;
				cellArr[cellIndex].index = cellIndex;
				cellArr[cellIndex].minX = x * cellEnd;
				cellArr[cellIndex].maxX = (x + 1) * cellEnd;

				cellArr[cellIndex].minY = y * cellEnd;
				cellArr[cellIndex].maxY = (y + 1) * cellEnd;

				cellArr[cellIndex].minZ = z * cellEnd;
				cellArr[cellIndex].maxZ = (z + 1) * cellEnd;
			}
		}
	}

	//add boids to grid
	for (auto b : boids)
	{
		glm::vec3 pos = b->GetPosition();

		int xNorm = (pos.x - gridMin) / cellSize;
		int yNorm = (pos.y - gridMin) / cellSize;
		int zNorm = (pos.z - gridMin) / cellSize;

		int index = xNorm * cellCountPerRow2 + yNorm * cellCountPerRow + zNorm;
		if(index > 0 && index < cellCount)
			cellArr[index].boidsInCell.push_back(b);
	}

	//check boids using grid

	unsigned long long sizeOfCellVec = sizeof(gridCell*) * 27;
	for (int x = 0; x < cellCountPerRow; x++)
	{
		for (int y = 0; y < cellCountPerRow; y++)
		{
			for (int z = 0; z < cellCountPerRow; z++)
			{
				int cellIndex = x * cellCountPerRow2 + y * cellCountPerRow + z;

				//std::vector<gridCell*> cellsToCheck;
				//cellsToCheck.reserve(sizeOfCellVec);
				std::vector<Boid*> boidsToCheck;


				//int numBoids = 0;
				for (int i : {-1, 0, 1})
				{
					int newX = x + i;
					if (newX < 0 || newX >= cellCountPerRow)
						continue;
					for (int j : {-1, 0, 1})
					{
						int newY = y + j;
						if (newY < 0 || newY >= cellCountPerRow)
							continue;

						for (int k : {-1, 0, 1})
						{

							int newZ = z + k;
							if (newZ < 0 || newZ >= cellCountPerRow)
								continue;

							gridCell& cellToAdd = cellArr[newX * cellCountPerRow2 + newY * cellCountPerRow + newZ];
							//numBoids += cellToAdd.boidsInCell.size();
							//cellsToCheck.push_back(&cellToAdd);

							for (auto boidToAdd : cellToAdd.boidsInCell)
								boidsToCheck.push_back(boidToAdd);
						}
					}
				}

				//boidsToCheck.reserve(sizeof(Boid*) * numBoids);

				//for (auto cell : cellsToCheck)
				//{

				//}

				for (auto boid : cellArr[cellIndex].boidsInCell)
				{
					boid->CheckOthers(boidsToCheck);
				}
			}
		}
	}
}
