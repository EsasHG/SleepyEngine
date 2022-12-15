#include "Boid.h"
#include <TransformSystem.h>


void Boid::Update(double deltaTime)
{
	Sleepy::TransformSystem::SetPosition(*this, Sleepy::TransformSystem::GetPosition(*this) + glm::vec3(0.2f*deltaTime, 0.0f, 0.0f));
}
