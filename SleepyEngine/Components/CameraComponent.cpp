#include "CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Entity.h"
#include "TransformSystem.h"

namespace Sleepy
{

    glm::mat4 CameraComponent::GetViewMatrix()
    {
		glm::mat4 mat = TransformSystem::GetModelMatrix(*m_Entity);


		glm::vec3 pos = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
        return glm::lookAt(pos, pos + front, up);
    }

    const glm::vec3 CameraComponent::GetPosition()
    {
		return m_Entity->GetPosition();
    }

	void CameraComponent::UpdateVectors()
	{
		glm::vec3 direction;

		glm::vec3 rot = m_Entity->GetRotation();
		direction.x = cos(glm::radians(rot.x)) * cos(glm::radians(rot.y));
		direction.y = sin(glm::radians(rot.y));
		direction.z = sin(glm::radians(rot.x)) * cos(glm::radians(rot.y));
		front = glm::normalize(direction);

		right = glm::normalize(glm::cross(front, up));
	}

}