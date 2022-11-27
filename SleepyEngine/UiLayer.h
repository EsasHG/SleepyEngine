#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <vector>
#include "Components/TransformComponent.h"
class Window;
class TransformComponent;
class Entity;
typedef int ImGuiTreeNodeFlags;
class UiLayer
{
public:
	UiLayer();
	void BeginFrame();
	void Run(double deltaTime, Entity* sceneEntity);

	void EndFrame();
	glm::vec3 clearColor = glm::vec3(0.7f, 0.3f, 0.6f);
	glm::vec3 quadColor = glm::vec3(0.4f, 0.4f, 0.4f);
	glm::vec3 pointLightPos = glm::vec3(0.4f, 0.4f, 0.4f);
	glm::vec3 pointLightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 dirLightDir = glm::vec3(-0.2f, -0.6f, -0.3f);
	glm::vec3 dirLightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);

	unsigned int sceneTexture;
	glm::vec2 contentRegionSize;

	int id;
private:
	//object tree window
	void SetupObjectTree(Entity* sceneEntity);
	void ProcessTreeNode(const ImGuiTreeNodeFlags& base_flags, Entity* entity);
	void ReorderObjectTree(Entity* newParent, int newIndex, Entity* entityToMove);

	//object window
	void SetupObjectWindow();
	void ShowTransformComp(TransformComponent* transform);
	void ShowDirLightComp(class DirLightComponent* dirLight);
	void ShowPointLightComp(class PointLightComponent* pointLight);


	bool showObjectWindow = true;
	bool showObjectTreeWindow = true;
	bool showRenderWindow = true;
	bool showTestWindow3 = true;
	bool pointLightSelected = false;
	bool sceneSelected = false;
	bool dirLightSelected = false;

	Entity* entityToSelect = nullptr;
	std::vector<Entity*> selectedEntities;

	Entity* entityToMove;
	Entity* newParentEntity;

	int newIndex =-1;
};
	static char inputText[128];

