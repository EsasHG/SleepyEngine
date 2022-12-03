#include "TransformSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Entity.h"


void TransformSystem::SetPosition(Entity* e, glm::vec3 pos)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	tComp->m_position = pos;

	RecalculateModelMatrices(tComp);
}

void TransformSystem::SetPosition(Entity* e, float posX, float posY, float posZ)
{
	glm::vec3 pos(posX, posY, posZ);
	SetPosition(e, pos);
}

void TransformSystem::SetScale(Entity* e, glm::vec3 scale)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	//tComp->m_scale = scale;
	tComp->m_scale = scale;
	RecalculateModelMatrices(tComp);
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

	RecalculateModelMatrices(tComp);

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
	
	//glm::vec3 scale;
	//glm::vec3 pos;
	//glm::quat rot;
	//glm::vec3 skew;
	//glm::vec4 persp;
	//glm::decompose(transformComp->m_modelMatrix, scale, rot, pos, skew, persp);
	//
	//return glm::eulerAngles(rot);
}
/*
glm::vec3 TransformSystem::GetLocalPosition(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetLocalPosition(tComp);
}

glm::vec3 TransformSystem::GetLocalPosition(const TransformComponent* transformComp)
{
	if (transformComp->m_parent)
	{
		glm::mat4 invParent = glm::inverse(transformComp->m_parent->m_modelMatrix);


		glm::vec3 scale;
		glm::vec3 pos;
		glm::quat rot;
		glm::vec3 skew;
		glm::vec4 persp;
		glm::decompose(invParent * transformComp->m_modelMatrix, scale, rot, pos, skew, persp);


		return pos;
	}
	else
		return GetPosition(transformComp);
}

glm::vec3 TransformSystem::GetLocalScale(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetLocalScale(tComp);
}

glm::vec3 TransformSystem::GetLocalScale(const TransformComponent* transformComp)
{
	if (transformComp->m_parent)
		return GetScale(transformComp) * GetScale(transformComp->m_parent);
	else
		return GetScale(transformComp);
}

glm::vec3 TransformSystem::GetLocalRotation(const Entity* e)
{
	auto* tComp = e->GetComponent<TransformComponent>();
	return GetLocalRotation(tComp);
}

glm::vec3 TransformSystem::GetLocalRotation(const TransformComponent* transformComp)
{
	if (transformComp->m_parent)
		return GetRotation(transformComp) + GetRotation(transformComp->m_parent);
	else
		return GetRotation(transformComp);
}
*/


// ********** Parenting **********
//this is somewhat bugged but should be easier to debug if i make som UI for it first.


void TransformSystem::SetParent(Entity* parent, Entity* child)
{
	auto* tParent = parent->GetComponent<TransformComponent>();
	auto* tChild = child->GetComponent<TransformComponent>();

	////finds positions
	//glm::vec3 globalPos, globalRot, globalScale;
	//globalPos = GetGlobalPosition(tChild);
	//globalRot = GetGlobalRotation(tChild);
	//globalScale = GetGlobalScale(tChild);

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



	tChild->m_modelMatrix = glm::inverse(tParent->m_modelMatrix) * tChild->m_modelMatrix;


	glm::vec3 scale;
	glm::vec3 pos;
	glm::quat rot;
	glm::vec3 skew;
	glm::vec4 persp;

	glm::decompose(tChild->m_modelMatrix, scale, rot, pos, skew, persp);

	tChild->m_position = pos;
	tChild->m_rotation = rot;
	tChild->m_scale = scale;

	RecalculateModelMatrices(tChild);

	//glm::vec3 pos = glm::vec3(newMat[0][0], newMat[1][0], newMat[2][0]);
	//Sets positions
	//SetPosition(child, globalPos - GetGlobalPosition(tParent));
	//SetRotation(child, globalRot - GetGlobalRotation(tParent));

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
		//globalPos = GetGlobalPosition(transform);
		//globalRot = GetGlobalRotation(transform);
		//globalScale = GetGlobalScale(transform);

		//removes from children
		auto* parent = transform->m_parent;                
		auto it = std::find(parent->m_children.begin(), parent->m_children.end(), transform);
		if (it != parent->m_children.end())
			parent->m_children.erase(it);
		
		//Sets parent to scene
		TransformComponent* sceneTransform = e->m_scene->m_SceneEntity->GetComponent<TransformComponent>();
		sceneTransform->m_children.push_back(transform);
		transform->m_parent = sceneTransform;
		

		transform->m_modelMatrix = glm::inverse(sceneTransform->m_modelMatrix) * transform->m_modelMatrix;


		glm::vec3 scale;
		glm::vec3 pos;
		glm::quat rot;
		glm::vec3 skew;
		glm::vec4 persp;

		glm::decompose(transform->m_modelMatrix, scale, rot, pos, skew, persp);

		transform->m_position = pos;
		transform->m_rotation = rot;
		transform->m_scale = scale;


		transform->m_modelMatrix = sceneTransform->m_modelMatrix * GetLocalModelMatrix(transform);
		RecalculateModelMatrices(transform);
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
	return transformComp->m_modelMatrix;
}

glm::mat4 TransformSystem::GetLocalModelMatrix(const TransformComponent* transformComp)
{ 
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, transformComp->m_position);
	model = model * glm::toMat4(transformComp->m_rotation);
	model = glm::scale(model, transformComp->m_scale);
	return model;
}

void TransformSystem::RecalculateModelMatrices(TransformComponent* transformComp)
{
	if (transformComp->m_parent)
	{
		transformComp->m_modelMatrix = transformComp->m_parent->m_modelMatrix * GetLocalModelMatrix(transformComp);
	}

	for (auto tChild : transformComp->m_children)
	{
		tChild->m_modelMatrix = transformComp->m_modelMatrix * GetLocalModelMatrix(tChild);
		RecalculateModelMatrices(tChild);
	}
}

//
//glm::mat4 TransformSystem::GetLocalModelMatrix(const Entity* e)
//{
//	auto* tComp = e->GetComponent<TransformComponent>();
//	return GetLocalModelMatrix(tComp);
//}
//
//glm::mat4 TransformSystem::GetLocalModelMatrix(const TransformComponent* transformComp)
//{
//	if (transformComp->m_parent)
//		return GetModelMatrix(transformComp->m_parent) * GetModelMatrix(transformComp);
//	else
//		return GetModelMatrix(transformComp);
//}
