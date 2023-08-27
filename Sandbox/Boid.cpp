#include "Boid.h"
#include <TransformSystem.h>
#include <Components/LightComponent.h>

void Boid::BeginPlay()
{
	Sleepy::Entity::BeginPlay();
}

void Boid::Update(double deltaTime)
{
	Sleepy::Entity::Update(deltaTime);
	
	glm::vec3 pos = GetPosition();

	if (pos.x < -boidInfo.margin)
		velocity.x = velocity.x + boidInfo.turnFactor;
	if (pos.x > boidInfo.margin)
		velocity.x = velocity.x - boidInfo.turnFactor;
	if (pos.y > boidInfo.margin)
		velocity.y = velocity.y - boidInfo.turnFactor;
	if (pos.y < -boidInfo.margin)
		velocity.y = velocity.y + boidInfo.turnFactor;
	if (pos.z > boidInfo.margin)
		velocity.z = velocity.z - boidInfo.turnFactor;
	if (pos.z < -boidInfo.margin)
		velocity.z = velocity.z + boidInfo.turnFactor;

	float speed2 = glm::abs(glm::dot(velocity, velocity));
	if (speed2 == 0) return;
	if (speed2 > boidInfo.maxSpeed * boidInfo.maxSpeed)
		velocity = glm::normalize(velocity) * boidInfo.maxSpeed;
	if (speed2 < boidInfo.minSpeed * boidInfo.minSpeed)
		velocity = glm::normalize(velocity) * boidInfo.minSpeed;
	SetPosition(GetPosition() + (float(deltaTime) * velocity));
}

void Boid::CheckOthers(std::vector<Boid*>& boids)
{
	glm::vec3 pos = GetPosition();

	glm::vec3 avoidVec = glm::vec3(0);
	glm::vec3 avgDir = glm::vec3(0);
	glm::vec3 avgPos = glm::vec3(0);
	float neighbouringBoids = 0.0f;

	for (auto boid : boids)
	{
		if (boid == this)
			continue;

		glm::vec3 otherPos = boid->GetPosition();
		glm::vec3 dir = (pos - otherPos);
		float distance2 = glm::abs(glm::dot(dir, dir));

		if (distance2 < boidInfo.minDistance * boidInfo.minDistance)
		{
			avoidVec += dir;
		}
		else if (distance2 < boidInfo.viewRange * boidInfo.viewRange)
		{
			avgDir += boid->velocity;
			avgPos += boid->GetPosition();
			neighbouringBoids++;
		}
	}
	velocity += avoidVec * boidInfo.avoidFactor;
	if (neighbouringBoids > 0)
	{
		avgDir = avgDir / neighbouringBoids;
		velocity += (avgDir - velocity) * boidInfo.matchingFactor;
		avgPos += avgPos / neighbouringBoids;
		velocity += (avgPos - pos) * boidInfo.centeringFactor;
	}
	if (inGroup1)
		velocity = (1 - boidInfo.biasVal) * velocity + (boidInfo.biasVal * 1.0f);
	else if (inGroup2)
		velocity = (1 - boidInfo.biasVal) * velocity + (boidInfo.biasVal * -1.0f);


}
