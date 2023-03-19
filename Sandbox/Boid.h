#pragma once
#include <Entity.h>
#include <Components/MeshComponent.h>

struct BoidInfo
{
	float minDistance = 0.8f;
	float viewRange = 4.0f;
	float avoidFactor = 0.05f;
	float matchingFactor = 0.05f;
	float centeringFactor = 0.0005f;
	float margin = 10.0f;
	float turnFactor = 0.5f;
	float maxSpeed = 8.0f;
	float minSpeed = 4.0f;
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

