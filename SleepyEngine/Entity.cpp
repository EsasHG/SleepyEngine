#include "Entity.h"
#include "Components/RelationshipComponent.h"
#include "Components/TransformComponent.h"
#include "TransformSystem.h"
#include "Scene.h"

namespace Sleepy
{
	Entity::Entity(std::string entityName, SceneBase* scene) : m_Name(entityName), m_scene(scene)
	{
		m_entityHandle = m_scene->m_registry.create();
		transformComp = &AddComponent<TransformComponent>(); //TODO: Should all entities really have a TransformComponent? Scenes as well?
		relationshipComp = &AddComponent<RelationshipComponent>(m_scene->m_SceneEntity);
		if (m_scene->m_SceneEntity)		//will not be true when creating scene entity, but should always be true otherwise
			SetParent(*m_scene->m_SceneEntity);
	}


	Entity& Entity::GetParent()
	{
		return *m_scene->m_registry.get<RelationshipComponent>(relationshipComp->m_parent).m_Entity;
	}

	glm::vec3 Entity::GetPosition()
	{
		return transformComp->m_position;
		//return TransformSystem::GetPosition(GetComponent<TransformComponent>());
	}

	glm::vec3 Entity::GetRotation()
	{
		return TransformSystem::GetRotation(*transformComp);
		//return TransformSystem::GetRotation(GetComponent<TransformComponent>());
	}

	glm::vec3 Entity::GetScale()
	{
		return transformComp->m_scale;
		//return TransformSystem::GetScale(GetComponent<TransformComponent>());
	}

	void Entity::SetPosition(glm::vec3 pos)
	{
		TransformSystem::SetPosition(*transformComp, pos);
		//TransformSystem::SetPosition(*this, pos);
	}
	void Entity::SetRotation(glm::vec3 rot)
	{
		TransformSystem::SetRotation(*transformComp, rot);
	}
	void Entity::SetScale(glm::vec3 scale)
	{
		TransformSystem::SetScale(*transformComp, scale);
	}

	std::vector<Entity*> Entity::GetChildren()
	{
		std::vector<Entity*> children;
		entt::entity current = relationshipComp->m_firstChild;

		while (current != entt::null)
		{
			RelationshipComponent& rComp = m_scene->m_registry.get<RelationshipComponent>(current);
			children.push_back(rComp.m_Entity);
			current = rComp.m_next;
		}

		return children;
	}

	bool Entity::HasParent()
	{
		return (relationshipComp->m_parent != entt::null);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns>true if entity has at least one child</returns>
	bool Entity::HasChildren()
	{
		return (relationshipComp->m_firstChild != entt::null);
	}

	void Entity::SetParent(Entity& newParent)
	{
		
		RemoveAsChild();
		relationshipComp->m_parent = newParent.m_entityHandle;

		//add to new parent's children
		entt::entity next = newParent.GetComponent<RelationshipComponent>().m_firstChild;

		if (next == entt::null) //new parent has no children
		{
			newParent.relationshipComp->m_firstChild = m_entityHandle;
			relationshipComp->m_prev = entt::null;
		}
		else
		{
			entt::entity current = entt::null;
			//find last child
			while (next != entt::null)
			{
				current = next;
				next = m_scene->m_registry.get<RelationshipComponent>(current).m_next;
			}

			assert(current != entt::null);

			RelationshipComponent& newPrev = m_scene->m_registry.get<RelationshipComponent>(current);
			newPrev.m_next = m_entityHandle;
			relationshipComp->m_prev = newPrev.m_Entity->m_entityHandle;
		}
		relationshipComp->m_next = entt::null;

		TransformSystem::RecalculateModelMatrices(*transformComp);
	}

	void Entity::Unparent()
	{
		SetParent(*m_scene->m_SceneEntity);
	}

	/// <summary>
	/// Moves a child of this entity to another place in the child list.
	/// </summary>
	/// <param name="child">The child to move.</param>
	/// <param name="newIndex">The new placement of the child</param>
	void Entity::MoveChild(Entity& child, int newIndex)
	{
		//TODO: assert that child is actually a child of this entity
		//find old index
		int oldIndex = 0;

		int i = 0;
		for (Entity* c : GetChildren())
		{
			i++;
			if (*c == child)
			{
				oldIndex = i;
				break;
			}
		}
		assert(oldIndex > 0); //child is not a child of this entity! 
		child.RemoveAsChild();
		RelationshipComponent& rCompChild = *child.relationshipComp;
		entt::entity current = relationshipComp->m_firstChild;

		if (current == entt::null)	//parent has no children
		{
			relationshipComp->m_firstChild = child.m_entityHandle;
			rCompChild.m_prev = entt::null;
			rCompChild.m_next = entt::null;
		}
		else if (newIndex == 0)		//Should be first child
		{
			RelationshipComponent& newNext = m_scene->m_registry.get<RelationshipComponent>(current);
			assert(newNext.m_prev == entt::null);
			newNext.m_prev = child.m_entityHandle;
			rCompChild.m_next = newNext.m_Entity->m_entityHandle;
			relationshipComp->m_firstChild = child.m_entityHandle;
			rCompChild.m_prev = entt::null;
		}
		else						//Somewhere in the list of children
		{
			//if newIndex is bigger than the oldIndex, the entity to move was counted when finding the newIndex
			if (newIndex > oldIndex)
				newIndex = newIndex--;

			for (int i = 2; i <= newIndex; i++)
			{
				current = m_scene->m_registry.get<RelationshipComponent>(current).m_next;
			}

			assert(current != entt::null); //newIndex out of range!

			RelationshipComponent& newPrev = m_scene->m_registry.get<RelationshipComponent>(current);
			if (newPrev.m_next != entt::null)
			{
				RelationshipComponent& newNext = m_scene->m_registry.get<RelationshipComponent>(newPrev.m_next);
				newNext.m_prev = child.m_entityHandle;
				rCompChild.m_next = newNext.m_Entity->m_entityHandle;
			}
			else
			{
				rCompChild.m_next = entt::null;
			}
			newPrev.m_next = child.m_entityHandle;
			rCompChild.m_prev = newPrev.m_Entity->m_entityHandle;
		}
	}


	Scene* Entity::GetScene()
	{
		return static_cast<Scene*>(m_scene);
	}

	/// <summary>
	/// removes itself from parent's children
	/// </summary>
	void Entity::RemoveAsChild()
	{
		if (relationshipComp->m_parent != entt::null)	//if we have a parent, remove from parent's children
		{
			if (relationshipComp->m_next != entt::null)	//if we have a next sibling
			{
				RelationshipComponent& rCompNextChild = m_scene->m_registry.get<RelationshipComponent>(relationshipComp->m_next);
				assert(rCompNextChild.m_prev == m_entityHandle);
				rCompNextChild.m_prev = relationshipComp->m_prev;
			}
			if (relationshipComp->m_prev != entt::null) //if we have a prev sibling
			{
				RelationshipComponent& rCompPrevChild = m_scene->m_registry.get<RelationshipComponent>(relationshipComp->m_prev);
				assert(rCompPrevChild.m_next == m_entityHandle);
				rCompPrevChild.m_next = relationshipComp->m_next;
			}
			else							//if first child
			{
				RelationshipComponent& rCompParent = m_scene->m_registry.get<RelationshipComponent>(relationshipComp->m_parent);
				assert(rCompParent.m_firstChild == m_entityHandle);
				rCompParent.m_firstChild = relationshipComp->m_next;	//new first child is next in list
			}
		}
	}
}