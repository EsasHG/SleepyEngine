#pragma once

namespace Sleepy
{
	enum COMPONENT_TYPE
	{
		TRANSFORM,
		MESH, 
		POINT_LIGHT,
		DIR_LIGHT,
		DELETION,
		RELATIONSHIP
	};
	
	class Component
	{
	public:
		Component() {};
	
		bool operator == (Component const& comp) 
	    {
			return (m_Entity == comp.m_Entity && m_componentType == comp.m_componentType);
	    }
	public:
	
		class Entity* m_Entity;
		COMPONENT_TYPE m_componentType;
	};
}

