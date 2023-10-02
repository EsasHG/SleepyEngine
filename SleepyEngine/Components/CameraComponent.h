#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace Sleepy
{
    struct BufferData
    {
        unsigned int framebuffer = 0;
        unsigned int renderbuffer = 0;
        unsigned int renderedTexture = 0;
        unsigned int bufferHeigth = 1920;
        unsigned int bufferWidth = 1080;
    };

    class CameraComponent : public Component
    {
    public:
        CameraComponent(Entity* entity);
        ~CameraComponent();
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
        BufferData m_BufferData;
    };

}