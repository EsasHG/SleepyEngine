#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>
//#include <queue>
#include <vector>
#include "Components/TransformComponent.h"

namespace Sleepy
{


	struct RenderWindow
	{
		unsigned int sceneTexture = 0;
		glm::vec2 contentRegionSize = glm::vec2(1920, 1080);
		int id = 0;
		std::string name = "New Render window";
		bool bOpen = true;
		bool bVisible = true;

		bool operator==(const RenderWindow& w) const
		{
			return id == w.id ? true : false;
		}
	};

	//info that is returned from the application from UiLayer::Run();
	struct UiInfo
	{
		std::vector<RenderWindow>& renderWindows;
		bool bTogglePlay = false;
		bool bNewWindow = false;
		bool bCreateScene = false;
		bool bCreateObject = false;
		class Scene* loadedScene = nullptr;
	};

	

	class Window;
	class TransformComponent;
	class Entity; 
	class Scene;
	typedef int ImGuiTreeNodeFlags;
	class UiLayer
	{
	public:
		UiLayer();
		void BeginFrame();
		UiInfo Run(double deltaTime, Scene* scene);

		void EndFrame();
		void ShowRenderWindow(RenderWindow& window);
	
		glm::vec3 clearColor = glm::vec3(0.7f, 0.3f, 0.6f);
		glm::vec3 quadColor = glm::vec3(0.4f, 0.4f, 0.4f);


		unsigned int sceneTexture;
		glm::vec2 contentRegionSize = glm::vec2(1920, 1080);

		int id;

	private:
		//object tree window
		void SetupObjectTree(Entity& sceneEntity);
		void ProcessTreeNode(const ImGuiTreeNodeFlags& base_flags, Entity& entity);
		void ReorderObjectTree(Entity& newParent, int newIndex, Entity& entityToMove);

		//object window
		void SetupObjectWindow(Scene* scene);
		void ShowTransformComp(Entity& entity);
		void ShowMeshComp(Entity& entity);
		void ShowDirLightComp(Entity& entity);
		void ShowPointLightComp(Entity& entity);
		void ShowCameraComp(Entity& entity);
		//assets window
		void SetupAssetsWindow();

		//window bools
		bool showObjectWindow = true;
		bool showObjectTreeWindow = true;
		bool showPerformanceWindow = true;
		bool showAssetsWindow = true;

		bool styleEditorOpen = false;
		bool engineSettingsOpen = false;
		bool userGuideOpen = false;
		bool metricsWindowOpen = false;
		bool debugLogWindowOpen = false;
		bool stackToolWindowOpen = false;
		bool playing;
		bool sceneSelected = false;

		//std::queue<float> frametimes;

		Entity* entityToSelect = nullptr;
		std::vector<entt::entity> selectedEntities;

		Entity* entityToMove;
		Entity* newParentEntity;
		//static float frames[140];
		int newIndex = -1;
		int oldIndex = -1;

		int windowID = 0;

		std::vector<RenderWindow> openRenderWindows;
		RenderWindow* windowToClose = nullptr;
	};
	static char inputText[128];
}
