#pragma once

#include <glm/glm.hpp>

namespace Sleepy
{
	class TransformComponent;
	class TransformSystem
	{

	public:
		// Should there be options other than using the Entity?
		static glm::vec3 GetPosition(const TransformComponent& transformComp);
		static glm::vec3 GetScale(const TransformComponent& transformComp);
		static glm::vec3 GetRotation(const TransformComponent& transformComp);

		static void SetPosition(TransformComponent& transformComp, float posX, float posY, float posZ);
		static void SetPosition(TransformComponent& transformComp, glm::vec3 pos);

		static void SetScale(TransformComponent& transformComp, float scaleX, float scaleY, float scaleZ);
		static void SetScale(TransformComponent& transformComp, glm::vec3 scale);

		static void SetRotation(TransformComponent& transformComp, float yaw, float pitch, float roll);
		static void SetRotation(TransformComponent& transformComp, glm::vec3 eulerRotDeg);

		static glm::mat4 GetModelMatrix(const TransformComponent& transformComp);
		static glm::mat4 GetLocalModelMatrix(const TransformComponent& transformComp);
		static void RecalculateModelMatrices(TransformComponent& transformComp);

	};
}
	