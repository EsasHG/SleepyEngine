#include "Scene.h"
#include "Components/InputComponent.h"
#include "InputManager.h"
namespace Sleepy
{

	//template<typename T, typename ...Args>
	//T& Scene::CreateEntityT(std::string entityName, Args && ...args)
	
	void Scene::BeginPlay()
	{
		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			it->second->bActive = true;
			it->second->BeginPlay();
		}
	}

	void Scene::EndPlay()
	{
		for (Entity* e : gameEntities)
			MarkForDeletion(*e);

		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			it->second->bActive = false;
		}
	}

	void Scene::Update(double deltaTime)
	{
		auto regView = m_registry.view<UpdateComponent>();
		for (auto [entity, up] : regView.each())
		{
			if(up.m_Entity->bActive)
				up.m_Entity->Update(deltaTime);
		}
	}
}