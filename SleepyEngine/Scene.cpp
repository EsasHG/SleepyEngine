#include "Scene.h"

namespace Sleepy
{

	//template<typename T, typename ...Args>
	//T& Scene::CreateEntityT(std::string entityName, Args && ...args)
	
	void Scene::BeginPlay()
	{
		for (auto it = entities.begin(); it != entities.end(); ++it)
			it->second->BeginPlay();
	}

	void Scene::Update(double deltaTime)
	{
		auto regView = m_registry.view<UpdateComponent>();
		for (auto [entity, up] : regView.each())
			up.m_Entity->Update(deltaTime);
	}

}