#include "Entity.h"
#include "Scene.h";
#include "Components/TransformComponent.h"

Entity::Entity(Scene* scene) : m_scene(scene)
{
	m_entityHandle = m_scene->m_registry.create();
}

