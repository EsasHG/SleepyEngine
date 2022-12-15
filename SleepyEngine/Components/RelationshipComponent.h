#pragma once
#include <entt/entt.hpp>
#include "Component.h"

namespace Sleepy
{
	class RelationshipComponent : public Component
	{
	public:
		RelationshipComponent(Entity* entity, Entity* parent);
	
		entt::entity m_firstChild{ entt::null };
		entt::entity m_parent{ entt::null };
		entt::entity m_prev{ entt::null };
		entt::entity m_next{ entt::null };
	
	};
}