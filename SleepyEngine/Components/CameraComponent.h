#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace Sleepy
{
    class CameraComponent :
        public Component
    {
    public:
        CameraComponent(Entity* entity)
        {
            m_Entity = entity;
            m_componentType = CAMERA;
        }

        glm::mat4 GetViewMatrix();
        const glm::vec3 GetPosition();

        void UpdateVectors();

        float fov = 0.5f;
        float renderDistance = 500.0f;

        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 right;
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        bool bPossessOnStart = true;
        Entity* lookatTarget = nullptr;
    };

}