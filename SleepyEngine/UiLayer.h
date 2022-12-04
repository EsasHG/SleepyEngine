#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
//#include <queue>
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
	
	bool RenderWindowOpen() { return renderWindowOpen; };
	glm::vec3 clearColor = glm::vec3(0.7f, 0.3f, 0.6f);
	glm::vec3 quadColor = glm::vec3(0.4f, 0.4f, 0.4f);

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
	void ShowMeshComp(class MeshComponent* mesh);
	void ShowDirLightComp(class DirLightComponent* dirLight);
	void ShowPointLightComp(class PointLightComponent* pointLight);
	//assets window
	void SetupAssetsWindow();

	//window bools
	bool showObjectWindow = true;
	bool showObjectTreeWindow = true;
	bool showRenderWindow = true;
	bool showPerformanceWindow = true;
	bool showAssetsWindow = true;

	bool styleEditorOpen = false;
	bool styleSelectorOpen = false;
	bool fontSelectorOpen = false;
	bool userGuideOpen = false;
	bool metricsWindowOpen = false;
	bool debugLogWindowOpen = false;
	bool stackToolWindowOpen = false;
	bool renderWindowOpen;

	bool sceneSelected = false;

	//std::queue<float> frametimes;

	Entity* entityToSelect = nullptr;
	std::vector<Entity*> selectedEntities;

	Entity* entityToMove;
	Entity* newParentEntity;
	//static float frames[140];
	int newIndex =-1;
};
	static char inputText[128];

