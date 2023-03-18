#include "TransformSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Entity.h"
#include "Components/TransformComponent.h"
namespace Sleepy
{


	// ********** Getting Values **********


	glm::vec3 TransformSystem::GetPosition(const TransformComponent& transformComp)
	{
		return transformComp.m_position;
	}

	glm::vec3 TransformSystem::GetScale(const TransformComponent& transformComp)
	{
		return transformComp.m_scale;
	}

	glm::vec3 TransformSystem::GetRotation(const TransformComponent& transformComp)
	{
		glm::vec3 rot = glm::eulerAngles(transformComp.m_rotation);
		rot.x = glm::degrees(rot.x);
		rot.y = glm::degrees(rot.y);
		rot.z = glm::degrees(rot.z);
		return rot;

		//glm::vec3 scale;
		//glm::vec3 pos;
		//glm::quat rot;
		//glm::vec3 skew;
		//glm::vec4 persp;
		//glm::decompose(transformComp->m_modelMatrix, scale, rot, pos, skew, persp);
		//
		//return glm::eulerAngles(rot);
	}


	// ********** Setting Values **********


	void TransformSystem::SetPosition(TransformComponent& transformComp, float posX, float posY, float posZ)
	{
		glm::vec3 pos(posX, posY, posZ);
		SetPosition(transformComp, pos);
	}
	void TransformSystem::SetPosition(TransformComponent& transformComp, glm::vec3 pos)
	{
		transformComp.m_position = pos;
		RecalculateModelMatrices(transformComp);
	}


	void TransformSystem::SetScale(TransformComponent& transformComp, float scaleX, float scaleY, float scaleZ)
	{
		glm::vec3 scale(scaleX, scaleY, scaleZ);
		SetScale(transformComp, scale);
	}
	void TransformSystem::SetScale(TransformComponent& transformComp, glm::vec3 scale)
	{
		transformComp.m_scale = scale;
		RecalculateModelMatrices(transformComp);
	}


	void TransformSystem::SetRotation(TransformComponent& transformComp, float yaw, float pitch, float roll)
	{
		glm::vec3 rot(yaw, pitch, roll);
		SetRotation(transformComp, rot);
	}
	void TransformSystem::SetRotation(TransformComponent& transformComp, glm::vec3 eulerRotDeg)
	{
		eulerRotDeg.x = glm::radians(eulerRotDeg.x);
		eulerRotDeg.y = glm::radians(eulerRotDeg.y);
		eulerRotDeg.z = glm::radians(eulerRotDeg.z);

		transformComp.m_rotation = glm::quat(eulerRotDeg);

		RecalculateModelMatrices(transformComp);
	}


	// ********** Matrix **********


	glm::mat4 TransformSystem::GetModelMatrix(const TransformComponent& transformComp)
	{
		return transformComp.m_modelMatrix;
	}

	glm::mat4 TransformSystem::GetLocalModelMatrix(const TransformComponent& transformComp)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transformComp.m_position);
		model = model * glm::toMat4(transformComp.m_rotation);
		model = glm::scale(model, transformComp.m_scale);
		return model;
	}

	void TransformSystem::RecalculateModelMatrices(TransformComponent& transformComp)
	{
		if (transformComp.m_Entity->HasParent())
		{
			TransformComponent* parentTransform = transformComp.m_Entity->GetParent().transformComp;
			transformComp.m_modelMatrix = parentTransform->m_modelMatrix * GetLocalModelMatrix(transformComp);
		}

		for (auto child : transformComp.m_Entity->GetChildren())
		{
			TransformComponent& tChild = *child->transformComp;
			//tChild.m_modelMatrix = transformComp.m_modelMatrix * GetLocalModelMatrix(tChild);		//is this neccessary?
			RecalculateModelMatrices(tChild);
		}
	}
}
