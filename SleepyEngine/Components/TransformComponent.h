#pragma once
#include "Component.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
class TransformComponent : Component
{
public:
	TransformComponent(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) :
		position(pos), rotation(rot), scale(scale) {};
private:

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	TransformComponent* parent = nullptr;

	std::vector<TransformComponent*> children;

	friend class TransformSystem;
};

