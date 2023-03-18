#pragma once

#include "SceneBase.h"
#include "Components/TransformComponent.h"
#include "Components/RelationshipComponent.h"
#include "Entity.h"
#include "Application.h"
namespace Sleepy
{
	struct UpdateComponent 
	{
		UpdateComponent(Entity* entity, int i = 0)
		{
			m_Entity = entity;
		}
		Entity* m_Entity;
	};

	class Scene : public SceneBase
	{
	public:
		Scene() : SceneBase() {}

		virtual void BeginPlay() override;
		virtual void Update(double deltaTime) override;

		template<typename T, typename ... Args>
		T& CreateGameObject(std::string entityName, Args&&... args)
		{
			bool convertible = std::is_nothrow_convertible<T*, Entity*>::value;
			assert(convertible);

			if (!m_SceneEntity)
			{
				CreateSceneEntity();
			}

			T* entity = new T(entityName, this, std::forward<Args>(args)...);
			//entity->AddComponent<TransformComponent>();
			//entity->AddComponent<RelationshipComponent>(m_SceneEntity);
			if (entity->enableUpdate)
				entity->AddComponent<UpdateComponent>(0);

			entities.insert({ entity->m_entityHandle, entity });
			if (Application::s_Playing)
			{
				entity->BeginPlay();
				gameEntities.push_back(entity);
			}

			return *entity;

		}
	};
}