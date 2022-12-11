#pragma once

#include <glm/glm.hpp>

class Entity;
class TransformComponent;
class TransformSystem
{

public:
	// Should there be options other than using the Entity?
	static void SetPosition(Entity& e, glm::vec3 pos);
	static void SetPosition(Entity& e, float posX, float posY, float posZ);

	static void SetScale(Entity& e, glm::vec3 scale);
	static void SetScale(Entity& e, float scaleX, float scaleY, float scaleZ);

	static void SetRotation(Entity& e, glm::vec3 rot);
	static void SetRotation(Entity& e, float yaw, float pitch, float roll);

	static glm::vec3 GetPosition(const Entity& e);
	static glm::vec3 GetPosition(const TransformComponent& transformComp);

	static glm::vec3 GetScale(const Entity& e);
	static glm::vec3 GetScale(const TransformComponent& transformComp);

	static glm::vec3 GetRotation(const Entity& e);
	static glm::vec3 GetRotation(const TransformComponent& transformComp);

	static glm::mat4 GetModelMatrix(const Entity& e);
	static glm::mat4 GetModelMatrix(const TransformComponent& transformComp);

	static glm::mat4 GetLocalModelMatrix(const TransformComponent& transformComp);
	static void RecalculateModelMatrices(TransformComponent& transformComp);
	static void RecalculateModelMatrices(Entity& entity);

private:

};

