#pragma once
#include "Component.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TransformComponent : public Component
{
public:
	TransformComponent(Entity* entity, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) : m_position(pos), m_rotation(rot), m_scale(scale) 
	{
		m_modelMatrix = glm::mat4(1.0f);
		m_modelMatrix = glm::translate(m_modelMatrix, pos);
		m_modelMatrix = m_modelMatrix * glm::toMat4(glm::quat(rot));
		m_modelMatrix = glm::scale(m_modelMatrix, scale);

		m_Entity = entity;
	};

private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_modelMatrix;
	TransformComponent* m_parent = nullptr;

	std::vector<TransformComponent*> m_children;

	friend class TransformSystem;
	friend class UiLayer;
};

