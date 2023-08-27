#pragma once
#include <Entity.h>
#include <Components/MeshComponent.h>

struct BoidInfo
{
	float minDistance = 1.0f;
	float viewRange = 6.92f;
	float avoidFactor = 0.348f;
	float matchingFactor = 0.10f;
	float centeringFactor = 0.0001f;
	float margin = 10.0f;
	float turnFactor = 0.770f;
	float maxSpeed = 50.0f;
	float minSpeed = 8.0f;
	float biasVal = 0.001;
};

class Boid : public Sleepy::Entity
{
public:
	Boid(std::string name, class Sleepy::SceneBase* scene, BoidInfo& info) 
		: Sleepy::Entity(name, scene), mesh(AddComponent<Sleepy::MeshComponent>("planet_0", "color")), boidInfo(info)
	{
		enableUpdate = true;
	}
	virtual void BeginPlay() override;
	virtual void Update(double deltaTime) override;
	void CheckOthers(std::vector<Boid*>& boids);
	Sleepy::MeshComponent& mesh;

	bool inGroup1 = false;
	bool inGroup2 = false;
	BoidInfo& boidInfo;
	glm::vec3 velocity = glm::vec3(0.0);

};

