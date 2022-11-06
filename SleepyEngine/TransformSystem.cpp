#include "TransformSystem.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Entity.h"


void TransformSystem::SetPosition(Entity* e, glm::vec3 pos)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	tComp->position = pos;
}

void TransformSystem::SetPosition(Entity* e, float posX, float posY, float posZ)
{
	glm::vec3 pos(posX, posY, posZ);
	SetPosition(e, pos);
}

void TransformSystem::SetScale(Entity* e, glm::vec3 scale)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	tComp->scale = scale;
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

	tComp->rotation = glm::quat(eulerRotDeg);
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
	return transformComp->position;
}

glm::vec3 TransformSystem::GetScale(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetScale(tComp);
}

glm::vec3 TransformSystem::GetScale(const TransformComponent* transformComp)
{
	return transformComp->scale;
}

glm::vec3 TransformSystem::GetRotation(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetRotation(tComp);
}

glm::vec3 TransformSystem::GetRotation(const TransformComponent* transformComp)
{
	glm::vec3 rot = glm::eulerAngles(transformComp->rotation);
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
	if (transformComp->parent)
		return GetPosition(transformComp) + GetGlobalPosition(transformComp->parent);
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
	if (transformComp->parent)
		return GetScale(transformComp) * GetGlobalScale(transformComp->parent);
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
	if (transformComp->parent)
		return GetRotation(transformComp) + GetGlobalRotation(transformComp->parent);
	else
		return GetRotation(transformComp);
}


// ********** Parenting **********
//this is somewhat bugged but should be easier to debug if i make som UI for it first.


void TransformSystem::SetParent(Entity* parent, Entity* child)
{
	auto* tParent = parent->GetComponent<TransformComponent>();
	auto* tChild = child->GetComponent<TransformComponent>();

	if (tChild->parent)
	{
		Unparent(child);
	}
	tParent->children.push_back(tChild);
	tChild->parent = tParent;
}

void TransformSystem::Unparent(Entity* e)
{
	auto* transform = e->GetComponent<TransformComponent>();
	if (transform->parent)
	{
		glm::vec3 globalPos, globalRot, globalScale;
		globalPos = GetGlobalPosition(transform);
		globalRot = GetGlobalRotation(transform);
		globalScale = GetGlobalScale(transform);


		auto* parent = transform->parent;
		auto it = std::find(parent->children.begin(), parent->children.end(), transform);
		if (it != parent->children.end())
			parent->children.erase(it);
		transform->parent = nullptr;
		
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
	if (transformComp->parent)
	{
		return GetModelMatrix(transformComp->parent) * GetLocalModelMatrix(transformComp);
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
	modelMat = glm::translate(modelMat, transformComp->position);
	modelMat = modelMat * glm::toMat4(transformComp->rotation);
	modelMat = glm::scale(modelMat, transformComp->scale);

	return modelMat;
}
