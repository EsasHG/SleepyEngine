#pragma once
#include <entt/entt.hpp>
#include "Component.h"
#include "Entity.h"

class RelationshipComponent : public Component
{
public:
	RelationshipComponent(Entity* entity, Entity* parent)
	{
		m_Entity = entity;
		m_componentType = RELATIONSHIP;
		if(parent)
			entity->SetParent(*parent);
	}

	entt::entity m_firstChild{ entt::null };
	entt::entity m_parent{ entt::null };
	entt::entity m_prev{ entt::null };
	entt::entity m_next{ entt::null };

};