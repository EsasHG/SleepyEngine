#pragma once
#include <vector>
#include <string>
namespace Sleepy
{
	static std::vector<std::string> validComponents = { "TransformComponent", "MeshComponent", "DirLightComponent", "PointLightComponent" };
	enum COMPONENT_TYPE
	{
		TRANSFORM,
		MESH, 
		POINT_LIGHT,
		DIR_LIGHT,
		INPUT,
		CAMERA,
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

