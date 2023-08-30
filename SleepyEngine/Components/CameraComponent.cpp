#include "CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Entity.h"

namespace Sleepy
{

    glm::mat4 CameraComponent::GetViewMatrix()
    {
		glm::vec3 pos = m_Entity->GetPosition();
        return glm::lookAt(pos, pos + front, up);
    }

    const glm::vec3 CameraComponent::GetPosition()
    {
		return m_Entity->GetPosition();
    }

	void CameraComponent::UpdateVectors()
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(direction);

		right = glm::normalize(glm::cross(front, up));
	}

}