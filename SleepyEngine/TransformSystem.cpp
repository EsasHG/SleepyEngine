#include "TransformSystem.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Entity.h"


void TransformSystem::SetPosition(Entity* e, glm::vec3 pos)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	tComp->m_position = pos;
}

void TransformSystem::SetPosition(Entity* e, float posX, float posY, float posZ)
{
	glm::vec3 pos(posX, posY, posZ);
	SetPosition(e, pos);
}

void TransformSystem::SetScale(Entity* e, glm::vec3 scale)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	tComp->m_scale = scale;
}


void TransformSystem::SetScale(Entity* e,float scaleX, float scaleY, float scaleZ)
{
	glm::vec3 scale(scaleX, scaleY, scaleZ);
	SetScale(e, scale);
}

void TransformSystem::SetRotation(Entity* e, glm::vec3 eulerRotDeg)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	
	eulerRotDeg.x = glm::radians(eulerRotDeg.x);
	eulerRotDeg.y = glm::radians(eulerRotDeg.y);
	eulerRotDeg.z = glm::radians(eulerRotDeg.z);

	tComp->m_rotation = glm::quat(eulerRotDeg);
}

void TransformSystem::SetRotation(Entity* e, float yaw, float pitch, float roll)
{
	auto* tComp = e->GetComponent<TransformComponent>();

	glm::vec3 rot(yaw, pitch, roll);

	SetRotation(e, rot);
}

glm::vec3 TransformSystem::GetPosition(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetPosition(tComp);
}

glm::vec3 TransformSystem::GetPosition(const TransformComponent* transformComp)
{
	return transformComp->m_position;
}

glm::vec3 TransformSystem::GetScale(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetScale(tComp);
}

glm::vec3 TransformSystem::GetScale(const TransformComponent* transformComp)
{
	return transformComp->m_scale;
}

glm::vec3 TransformSystem::GetRotation(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetRotation(tComp);
}

glm::vec3 TransformSystem::GetRotation(const TransformComponent* transformComp)
{
	glm::vec3 rot = glm::eulerAngles(transformComp->m_rotation);
	rot.x = glm::degrees(rot.x);
	rot.y = glm::degrees(rot.y);
	rot.z = glm::degrees(rot.z);
	return rot;
}


glm::vec3 TransformSystem::GetGlobalPosition(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetGlobalPosition(tComp);
}

glm::vec3 TransformSystem::GetGlobalPosition(const TransformComponent* transformComp)
{
	if (transformComp->m_parent)
		return GetPosition(transformComp) + GetGlobalPosition(transformComp->m_parent);
	else
		return GetPosition(transformComp);
}

glm::vec3 TransformSystem::GetGlobalScale(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetGlobalScale(tComp);
}

glm::vec3 TransformSystem::GetGlobalScale(const TransformComponent* transformComp)
{
	if (transformComp->m_parent)
		return GetScale(transformComp) * GetGlobalScale(transformComp->m_parent);
	else
		return GetScale(transformComp);
}

glm::vec3 TransformSystem::GetGlobalRotation(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetGlobalScale(tComp);
}

glm::vec3 TransformSystem::GetGlobalRotation(const TransformComponent* transformComp)
{
	if (transformComp->m_parent)
		return GetRotation(transformComp) + GetGlobalRotation(transformComp->m_parent);
	else
		return GetRotation(transformComp);
}


// ********** Parenting **********
//this is somewhat bugged but should be easier to debug if i make som UI for it first.


void TransformSystem::SetParent(Entity* parent, Entity* child)
{
	auto* tParent = parent->GetComponent<TransformComponent>();
	auto* tChild = child->GetComponent<TransformComponent>();

	//finds positions
	glm::vec3 globalPos, globalRot, globalScale;
	globalPos = GetGlobalPosition(tChild);
	globalRot = GetGlobalRotation(tChild);
	globalScale = GetGlobalScale(tChild);

	if (tChild->m_parent)
	{
		//unparents
		auto* oldParent = tChild->m_parent;
		auto it = std::find(oldParent->m_children.begin(), oldParent->m_children.end(), tChild);
		if (it != oldParent->m_children.end())
			oldParent->m_children.erase(it);
		tChild->m_parent = nullptr;
	}
	tParent->m_children.push_back(tChild);
	tChild->m_parent = tParent;
	glm::mat4 invParentMat = glm::inverse(GetModelMatrix(tParent));

	glm::mat4 newMat = invParentMat * GetModelMatrix(tChild);
	//glm::vec3 pos = glm::vec3(newMat[0][0], newMat[1][0], newMat[2][0]);
	//Sets positions
	SetPosition(child, globalPos - GetGlobalPosition(tParent));
	SetRotation(child, globalRot - GetGlobalRotation(tParent));

	//SetPosition(child, pos);
	//SetScale(child, globalScale - GetGlobalScale(tParent));

}

void TransformSystem::Unparent(Entity* e)
{
	auto* transform = e->GetComponent<TransformComponent>();
	if (transform->m_parent)
	{
		//finds positions
		glm::vec3 globalPos, globalRot, globalScale;
		globalPos = GetGlobalPosition(transform);
		globalRot = GetGlobalRotation(transform);
		globalScale = GetGlobalScale(transform);

		//removes from children
		auto* parent = transform->m_parent;                
		auto it = std::find(parent->m_children.begin(), parent->m_children.end(), transform);
		if (it != parent->m_children.end())
			parent->m_children.erase(it);
		
		//Sets parent to scene
		TransformComponent* sceneTransform = e->m_scene->m_SceneEntity->GetComponent<TransformComponent>();
		sceneTransform->m_children.push_back(transform);
		transform->m_parent = sceneTransform;
		
		//Sets positions
		SetPosition(e, globalPos);
		SetRotation(e, globalRot);
		SetScale(e, globalScale);
	}
}

// ********** Matrix **********


glm::mat4 TransformSystem::GetModelMatrix(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetModelMatrix(tComp);
}

glm::mat4 TransformSystem::GetModelMatrix(const TransformComponent* transformComp)
{
	if (transformComp->m_parent)
	{
		return GetModelMatrix(transformComp->m_parent) * GetLocalModelMatrix(transformComp);
	}
	else
		return GetLocalModelMatrix(transformComp);
}


glm::mat4 TransformSystem::GetLocalModelMatrix(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetLocalModelMatrix(tComp);
}

glm::mat4 TransformSystem::GetLocalModelMatrix(const TransformComponent* transformComp)
{
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, transformComp->m_position);
	modelMat = modelMat * glm::toMat4(transformComp->m_rotation);
	modelMat = glm::scale(modelMat, transformComp->m_scale);

	return modelMat;
}
