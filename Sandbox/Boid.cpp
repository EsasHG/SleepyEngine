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

	glm::vec3 avoidVec = glm::vec3(0);
	glm::vec3 avgDir = glm::vec3(0);
	glm::vec3 avgPos = glm::vec3(0);
	float neighbouringBoids = 0.0f;

	for (auto boid : otherBoids)
	{
		if (boid == this)
			continue;

		glm::vec3 otherPos = boid->GetPosition();
		glm::vec3 dir = (pos - otherPos);
		float distance2 = glm::abs(glm::dot(dir, dir));

		if (distance2 < minDistance* minDistance)
		{
			avoidVec += dir;
		}
		else if(distance2 < viewRange * viewRange)
		{
			avgDir += boid->direction;
			avgPos += boid->GetPosition();
			neighbouringBoids++;
		}
	}
	velocity += avoidVec * avoidFactor;
	if (neighbouringBoids > 0)
	{
		avgDir = avgDir / neighbouringBoids;
		velocity += (avgDir - velocity)*matchingFactor;
		avgPos += avgPos / neighbouringBoids;
		velocity += (avgPos - pos) * centeringFactor;
	}


	if (pos.x < -margin)
		velocity.x = velocity.x + turnFactor;
	if (pos.x > margin)
		velocity.x = velocity.x - turnFactor;
	if(pos.y > margin)
		velocity.y = velocity.y - turnFactor;
	if (pos.y < -margin)
		velocity.y = velocity.y + turnFactor;
	if (pos.z > margin)
		velocity.z = velocity.z - turnFactor;
	if (pos.z < -margin)
		velocity.z = velocity.z + turnFactor;

	float speed2 = glm::abs(glm::dot(velocity, velocity));
	if (speed2 > maxSpeed * maxSpeed)
		velocity = glm::normalize(velocity) * maxSpeed;
	if(speed2 < minSpeed*minSpeed)
		velocity = glm::normalize(velocity) * minSpeed;

	SetPosition(pos + float(deltaTime)* velocity *1.0f);
}
