#pragma once
#include "Component.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
class TransformComponent : public Component
{
public:
	TransformComponent(Entity* entity, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) :
		position(pos), rotation(rot), scale(scale) 
	{
		m_Entity = entity;
	};

	std::string m_Name;

private:
	//TODO: add m_ in variable names here?
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	TransformComponent* parent = nullptr;

	std::vector<TransformComponent*> children;

	friend class TransformSystem;
	friend class UiLayer;
};

