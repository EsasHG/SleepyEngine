#pragma once
#include "Component.h"

#include <glm/glm.hpp>
namespace Sleepy
{

    class LightComponent : public Component
    {
    public:
        LightComponent(Entity* entity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : 
            m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
        {
            m_Entity = entity;
        };
        
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
    
    };
    
    
    class DirLightComponent : public LightComponent
    {
    public:
        DirLightComponent(Entity* entity, glm::vec3 ambient = glm::vec3(0.1f), glm::vec3 diffuse = glm::vec3(0.5f), glm::vec3 specular = glm::vec3(1.0f), glm::vec3 direction = glm::vec3(0.1f, -0.8f,0.2f)) :
            LightComponent(entity, ambient, diffuse, specular), m_direction(direction)
        {
            m_componentType = DIR_LIGHT;
        };
    
        glm::vec3 m_direction;
    };
    
    class PointLightComponent : public LightComponent
    {
    public:
        PointLightComponent(Entity* entity, glm::vec3 ambient = glm::vec3(0.0f), glm::vec3 diffuse = glm::vec3(0.5f), glm::vec3 specular = glm::vec3(1.0f), float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f) :
            LightComponent(entity, ambient, diffuse, specular), m_constant(constant), m_linear(linear), m_quadratic(quadratic)
        {
            m_componentType = COMPONENT_TYPE::POINT_LIGHT;
        };
    
        float m_constant;
        float m_linear;
        float m_quadratic;
    
    };
}