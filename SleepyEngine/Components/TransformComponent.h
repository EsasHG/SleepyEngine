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
		m_position(pos), m_rotation(rot), m_scale(scale) 
	{
		m_Entity = entity;
	};

private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;
	TransformComponent* m_parent = nullptr;

	std::vector<TransformComponent*> m_children;

	friend class TransformSystem;
	friend class UiLayer;
};

