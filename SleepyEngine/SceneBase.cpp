#include "SceneBase.h"

#include "Entity.h"
#include "Components/MeshComponent.h"
#include "Components/LightComponent.h"
#include "Components/RelationshipComponent.h"
#include "Components/TransformComponent.h"
#include "Application.h"
namespace Sleepy
{
	class Deletion : Component
	{
	public:
		Deletion(Entity* entity, int d)
		{
			m_Entity = entity;
			m_componentType = DELETION;
		}
		COMPONENT_TYPE typeToRemove;
		friend class SceneBase;
	};

	SceneBase::SceneBase()
	{

	}

	SceneBase::~SceneBase()
	{
	}

	void SceneBase::CreateSceneEntity()
	{
		m_SceneEntity = new Entity(std::string("Scene"), this);
		//m_SceneEntity = std::make_unique<Entity>(new Entity(std::string("Scene"), this));
		m_SceneEntity->AddComponent<TransformComponent>();				//TODO: make this not neccessary. Still need it for rendering i think
		m_SceneEntity->AddComponent<RelationshipComponent>(nullptr);
	}

	Entity& SceneBase::CreateEntity(std::string entityName)
	{
		if (!m_SceneEntity)
		{
			CreateSceneEntity();
		}
		Entity* entity = new Entity(entityName, this);
		entity->AddComponent<TransformComponent>();
		entity->AddComponent<RelationshipComponent>(m_SceneEntity);

		entities.insert({ entity->m_entityHandle, entity });
		//entities.insert(entity->m_entityHandle,entity);
		if (Application::s_Playing)
		{
			entity->BeginPlay();
			gameEntities.push_back(entity);
		}
		return *entity;
	}

	Entity* SceneBase::GetEntity(entt::entity entityHandle)
	{
		auto it = entities.find(entityHandle);
		if (it != entities.end())
			return it->second;
		else
			return nullptr;
		// TODO: insert return statement here
	}

	//have not been able to test this with groups that have multiple children
	Entity& SceneBase::LoadMeshGroup(MeshGroup* meshGroup, Entity& parent, std::string groupName)
	{
		//TODO: use reference instead of pointer
		Entity* ent = nullptr;
		MeshRef* currentChild = meshGroup->firstChild;
		MeshGroup* currentChildGroup = meshGroup->firstChildGroup;

		if (currentChild && !currentChild->nextChildMesh) // if only one child mesh, add mesh to this entity
		{
			ent = &CreateEntity(groupName);
			ent->SetParent(parent);
			ent->AddComponent<MeshComponent>(currentChild->meshName, currentChild->materialName);
		}
		else if (currentChild)
		{
			ent = &CreateEntity(groupName);
			ent->SetParent(parent);

			while (currentChild)
			{
				Entity* meshEnt = &CreateEntity(currentChild->meshName);
				meshEnt->AddComponent<MeshComponent>(currentChild->meshName, currentChild->materialName);
				ent->SetParent(*ent);
				currentChild = currentChild->nextChildMesh;
			}
		}
		else
		{
			if (currentChildGroup && !currentChildGroup->nextGroup)
			{
				ent = &LoadMeshGroup(currentChildGroup, parent, groupName + "_" + std::to_string(0));
				return *ent;
			}
			else
				ent = &CreateEntity(groupName);
		}

		int i = 0;
		while (currentChildGroup)
		{
			Entity* entity = &LoadMeshGroup(currentChildGroup, *ent, groupName + "_" + std::to_string(i));
			currentChildGroup = currentChildGroup->nextGroup;
			i++;
		}

		return *ent;
	}

	void SceneBase::Draw()
	{
		//update light values. I will figure out something smarter for this later lmao
		unsigned int shaderID = ModelLibrary::GetInstance().GetShader("default");
		auto pointLightView = m_registry.view<TransformComponent, PointLightComponent>();
		for (auto [entity, transform, light] : pointLightView.each())
			Renderer::SetPointLightValues(shaderID, transform, light);

		auto dirLightView = m_registry.view<TransformComponent, DirLightComponent>();
		for (auto [entity, transform, light] : dirLightView.each())
			Renderer::SetDirLightValues(shaderID, transform, light);

		//draw objects
		auto regView = m_registry.view<TransformComponent, MeshComponent>();
		for (auto [entity, transform, mesh] : regView.each())
			Renderer::DrawMesh(mesh, transform);

	}

	void SceneBase::CleanupRegistry()
	{
		auto regView = m_registry.view<Deletion>();
		for (auto [entity, del] : regView.each())
			DeleteEntity(*del.m_Entity);
	}

	void SceneBase::MarkForDeletion(Entity& entity)
	{
		if(!entity.Has<Deletion>())
			entity.AddComponent<Deletion>(2);
		auto& rComp = entity.GetComponent<RelationshipComponent>();

		for (auto child : entity.GetChildren())
		{
			MarkForDeletion(*child);
		}
	}

	void SceneBase::DeleteEntity(Entity& entity)
	{
		entity.RemoveAsChild();

		m_registry.destroy(entity.m_entityHandle);

		auto it = entities.find(entity.m_entityHandle);
		Entity* ent = nullptr;
		if (it != entities.end())
		{
			Entity* ent = it->second;
			entities.erase(it);		
		}

		auto it2 = std::find(gameEntities.begin(), gameEntities.end(), &entity);
		if (it2 != gameEntities.end())
		{
			gameEntities.erase(it2);
		}
		delete ent;
	}

	void SceneBase::DeleteAllComponents(Entity& entity)
	{

	}

	template<typename T>
	void SceneBase::RemoveComponent(Entity* entity)
	{
		m_registry.remove<T>(entity->m_entityHandle);
	}

}