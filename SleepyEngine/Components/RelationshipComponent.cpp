#include "RelationshipComponent.h"
#include "Entity.h"

namespace Sleepy
{
	RelationshipComponent::RelationshipComponent(Entity* entity, Entity* parent)
	{
		m_Entity = entity;
		m_componentType = RELATIONSHIP;
		if (parent)
			entity->SetParent(*parent);
	}
}