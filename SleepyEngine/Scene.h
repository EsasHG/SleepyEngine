#pragma once
#include <entt/entt.hpp>

class Entity;
class Scene
{
public:
	Scene();

	void Update();
private:
	Entity* CreateEntity();


	entt::registry m_registry;

	friend class Entity;
};

