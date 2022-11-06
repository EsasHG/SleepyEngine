#include "Entity.h"
#include "Scene.h";
#include "Components/TransformComponent.h"

Entity::Entity(std::string entityName, Scene* scene) : m_Name(entityName), m_scene(scene)
{
	m_entityHandle = m_scene->m_registry.create();
}

