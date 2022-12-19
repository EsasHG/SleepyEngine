#include "Boid.h"
#include <TransformSystem.h>
#include <Components/LightComponent.h>

void Boid::BeginPlay()
{
	Sleepy::Entity::BeginPlay();
	AddComponent<Sleepy::PointLightComponent>();
}

void Boid::Update(double deltaTime)
{
	Sleepy::Entity::Update(deltaTime);

	SetPosition(GetPosition() + glm::vec3(0.2f * deltaTime, 0.0f, 0.0f));
}
