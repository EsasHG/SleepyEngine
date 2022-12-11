#include "Entity.h"
#include "Components/RelationshipComponent.h"
#include "TransformSystem.h"

	Entity& Entity::GetParent()
	{
		return *m_scene->m_registry.get<RelationshipComponent>(GetComponent<RelationshipComponent>().m_parent).m_Entity;
	}


	std::vector<Entity*> Entity::GetChildren()
	{
		std::vector<Entity*> children;
		entt::entity current = GetComponent<RelationshipComponent>().m_firstChild;

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
		return (GetComponent<RelationshipComponent>().m_parent != entt::null);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns>true if entity has at least one child</returns>
	bool Entity::HasChildren()
	{
		return (GetComponent<RelationshipComponent>().m_firstChild != entt::null);
	}

	void Entity::SetParent(Entity& newParent)
	{
		RelationshipComponent& rComp = GetComponent<RelationshipComponent>();
		RemoveAsChild();
		rComp.m_parent = newParent.m_entityHandle;

		//add to new parent's children
		entt::entity next = newParent.GetComponent<RelationshipComponent>().m_firstChild;

		if (next == entt::null) //new parent has no children
		{
			newParent.GetComponent<RelationshipComponent>().m_firstChild = m_entityHandle;
			rComp.m_prev = entt::null;
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
			rComp.m_prev = newPrev.m_Entity->m_entityHandle;
		}
		rComp.m_next = entt::null;

		TransformSystem::RecalculateModelMatrices(*this);
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
		RelationshipComponent& rComp = GetComponent<RelationshipComponent>();
		RelationshipComponent& rCompChild = child.GetComponent<RelationshipComponent>();
		entt::entity current = rComp.m_firstChild;

		if (current == entt::null)	//parent has no children
		{
			rComp.m_firstChild = child.m_entityHandle;
			rCompChild.m_prev = entt::null;
			rCompChild.m_next = entt::null;
		}
		else if (newIndex == 0)		//Should be first child
		{
			RelationshipComponent& newNext = m_scene->m_registry.get<RelationshipComponent>(current);
			assert(newNext.m_prev == entt::null);
			newNext.m_prev = child.m_entityHandle;
			rCompChild.m_next = newNext.m_Entity->m_entityHandle;
			rComp.m_firstChild = child.m_entityHandle;
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

	/// <summary>
	/// removes itself from parent's children
	/// </summary>
	void Entity::RemoveAsChild()
	{
		RelationshipComponent& rComp = GetComponent<RelationshipComponent>();
		if (rComp.m_parent != entt::null)	//if we have a parent, remove from parent's children
		{
			if (rComp.m_next != entt::null)	//if we have a next sibling
			{
				RelationshipComponent& rCompNextChild = m_scene->m_registry.get<RelationshipComponent>(rComp.m_next);
				assert(rCompNextChild.m_prev == m_entityHandle);
				rCompNextChild.m_prev = rComp.m_prev;
			}
			if (rComp.m_prev != entt::null) //if we have a prev sibling
			{
				RelationshipComponent& rCompPrevChild = m_scene->m_registry.get<RelationshipComponent>(rComp.m_prev);
				assert(rCompPrevChild.m_next == m_entityHandle);
				rCompPrevChild.m_next = rComp.m_next;
			}
			else							//if first child
			{
				RelationshipComponent& rCompParent = m_scene->m_registry.get<RelationshipComponent>(rComp.m_parent);
				assert(rCompParent.m_firstChild == m_entityHandle);
				rCompParent.m_firstChild = rComp.m_next;	//new first child is next in list
			}
		}
	}