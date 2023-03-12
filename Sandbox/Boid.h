#pragma once
#include <Entity.h>
#include <Components/MeshComponent.h>

class Boid : public Sleepy::Entity
{
public:
	Boid(std::string name, class Sleepy::SceneBase* scene, std::vector<Boid*>& boids) : Sleepy::Entity(name, scene), mesh(AddComponent<Sleepy::MeshComponent>("planet_0", "color", "color")), otherBoids(boids)
	{
		enableUpdate = true;
	}
	virtual void BeginPlay() override;
	virtual void Update(double deltaTime) override;
	Sleepy::MeshComponent& mesh;

	float minDistance = 1.2f;
	float viewRange = 6.0f;
	float avoidFactor = 0.5f;
	float matchingFactor= 0.2f;
	float centeringFactor = 0.004f;
	float margin = 10.0f;
	float turnFactor = 0.8f;
	float maxSpeed = 10.0f;
	float minSpeed = 5.0f;
	
	glm::vec3 direction = glm::vec3(0);
	glm::vec3 velocity = glm::vec3(0.1);
	std::vector<Boid*> &otherBoids;
	

};

