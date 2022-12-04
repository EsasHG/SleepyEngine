#pragma once


enum COMPONENT_TYPES
{
	TRANSFORM,
	MESH, 
	POINT_LIGHT,
	DIR_LIGHT,
	DELETION
};

class Component
{
public:
	Component() {};

	bool operator == (Component const& comp) 
    {

		return (m_Entity == comp.m_Entity && m_componentType == comp.m_componentType);
    }

protected:
	class Entity* m_Entity;
	COMPONENT_TYPES m_componentType;
};

