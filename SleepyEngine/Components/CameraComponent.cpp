#include "CameraComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include<glm/gtc/quaternion.hpp>

#include "Entity.h"
#include "TransformSystem.h"

namespace Sleepy
{

    glm::mat4 CameraComponent::GetViewMatrix()
    {
		glm::vec3 pos = m_Entity->GetWorldPosition();
		if(lookatTarget)
	        return glm::lookAt(pos, lookatTarget->GetPosition(), up);
		else
			return glm::lookAt(pos, pos + front, up);

    }

    const glm::vec3 CameraComponent::GetPosition()
    {
		return m_Entity->GetPosition();
    }

	void CameraComponent::UpdateVectors()
	{

		/*glm::mat4 model = TransformSystem::GetModelMatrix(*m_Entity);
		glm::vec3 parentRot = m_Entity->GetParent().GetRotation();
		float scaleFactor = sqrt(model[0][0] * model[0][0] + model[0][1] * model[0][1] + model[0][2] * model[0][2]);

		glm::mat3 rotMat = (1.0f / scaleFactor) * 
			glm::mat3(
			model[0][0], model[0][1], model[0][2],
			model[1][0], model[1][1], model[1][2],
			model[2][0], model[2][1], model[2][2]
		);
		glm::quat quatRot= glm::quat_cast(rotMat);
		
		glm::vec3 rot = glm::eulerAngles(quatRot);
		float oldX = rot.x;
		rot.x = glm::degrees(rot.y);
		rot.y = glm::degrees(-oldX);
		rot.z = glm::degrees(rot.z);*/

		/*rot = glm::rotateX(rot,parentRot.x);
		rot = glm::rotateY(rot,parentRot.y);
		rot = glm::rotateZ(rot,parentRot.z);*/
		
		glm::vec3 direction;
		if (lookatTarget)
		{

			front = glm::normalize(lookatTarget->GetPosition() - m_Entity->GetWorldPosition());
		}
		else
		{
			glm::vec3 rot = m_Entity->GetRotation();

			direction.x = cos(glm::radians(rot.x)) * cos(glm::radians(rot.y));
			direction.y = sin(glm::radians(rot.y));
			direction.z = sin(glm::radians(rot.x)) * cos(glm::radians(rot.y));
			
			front = glm::normalize(direction);
		}

		right = glm::normalize(glm::cross(front, up));
	}

}