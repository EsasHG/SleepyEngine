#include "TransformSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Entity.h"
#include "Components/TransformComponent.h"
namespace Sleepy
{


	void TransformSystem::SetPosition(Entity& e, glm::vec3 pos)
	{
		TransformComponent& tComp = e.GetComponent<TransformComponent>();
		tComp.m_position = pos;
		RecalculateModelMatrices(tComp);
	}

	void TransformSystem::SetPosition(Entity& e, float posX, float posY, float posZ)
	{
		glm::vec3 pos(posX, posY, posZ);
		SetPosition(e, pos);
	}

	void TransformSystem::SetScale(Entity& e, glm::vec3 scale)
	{
		auto& tComp = e.GetComponent<TransformComponent>();
		tComp.m_scale = scale;
		RecalculateModelMatrices(tComp);
	}


	void TransformSystem::SetScale(Entity& e, float scaleX, float scaleY, float scaleZ)
	{
		glm::vec3 scale(scaleX, scaleY, scaleZ);
		SetScale(e, scale);
	}

	void TransformSystem::SetRotation(Entity& e, glm::vec3 eulerRotDeg)
	{
		auto& tComp = e.GetComponent<TransformComponent>();

		eulerRotDeg.x = glm::radians(eulerRotDeg.x);
		eulerRotDeg.y = glm::radians(eulerRotDeg.y);
		eulerRotDeg.z = glm::radians(eulerRotDeg.z);

		tComp.m_rotation = glm::quat(eulerRotDeg);

		RecalculateModelMatrices(tComp);

	}

	void TransformSystem::SetRotation(Entity& e, float yaw, float pitch, float roll)
	{
		auto& tComp = e.GetComponent<TransformComponent>();

		glm::vec3 rot(yaw, pitch, roll);

		SetRotation(e, rot);
	}

	glm::vec3 TransformSystem::GetPosition(const Entity& e)
	{
		auto& tComp = e.GetComponent<TransformComponent>();
		return GetPosition(tComp);
	}

	glm::vec3 TransformSystem::GetPosition(const TransformComponent& transformComp)
	{
		return transformComp.m_position;
	}

	glm::vec3 TransformSystem::GetScale(const Entity& e)
	{
		auto& tComp = e.GetComponent<TransformComponent>();
		return GetScale(tComp);
	}

	glm::vec3 TransformSystem::GetScale(const TransformComponent& transformComp)
	{
		return transformComp.m_scale;
	}

	glm::vec3 TransformSystem::GetRotation(const Entity& e)
	{
		auto& tComp = e.GetComponent<TransformComponent>();
		return GetRotation(tComp);
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

	// ********** Matrix **********


	glm::mat4 TransformSystem::GetModelMatrix(const Entity& e)
	{
		auto tComp = e.GetComponent<TransformComponent>();
		return GetModelMatrix(tComp);
	}

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
			TransformComponent& parentTransform = transformComp.m_Entity->GetParent().GetComponent<TransformComponent>();
			transformComp.m_modelMatrix = parentTransform.m_modelMatrix * GetLocalModelMatrix(transformComp);
		}

		for (auto child : transformComp.m_Entity->GetChildren())
		{
			TransformComponent& tChild = child->GetComponent<TransformComponent>();
			tChild.m_modelMatrix = transformComp.m_modelMatrix * GetLocalModelMatrix(tChild);
			RecalculateModelMatrices(tChild);
		}
	}

	void TransformSystem::RecalculateModelMatrices(Entity& entity)
	{
		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		RecalculateModelMatrices(transform);
	}
}
