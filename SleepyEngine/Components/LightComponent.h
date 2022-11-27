#pragma once
#include "Component.h"

#include <glm/glm.hpp>
enum LightType
{
    DIRECTIONAL_LIGHT,
    POINT_LIGHT,
    SPOTLIGHT
};

class LightComponent : public Component
{
public:
    LightComponent(Entity* entity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, LightType type) : 
        m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_type(type)
    {
        m_Entity = entity;
    };
    
    LightType m_type;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;

};


class DirLightComponent : public LightComponent
{
public:
    DirLightComponent(Entity* entity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) :
        LightComponent(entity, ambient, diffuse, specular, DIRECTIONAL_LIGHT), m_direction(direction)
    {
    };

    glm::vec3 m_direction;
};

class PointLightComponent : public LightComponent
{
public:
    PointLightComponent(Entity* entity, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position = glm::vec3(0.0f), float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f) :
        LightComponent(entity, ambient, diffuse, specular, POINT_LIGHT), m_position(position), m_constant(constant), m_linear(linear), m_quadratic(quadratic)
    {
    };

    glm::vec3 m_position;
    float m_constant;
    float m_linear;
    float m_quadratic;

};