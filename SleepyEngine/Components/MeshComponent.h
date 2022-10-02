#pragma once
#include "Component.h"
#include <string>

#include "Mesh.h"
#include "Renderer.h"
#include "ModelLibrary.h"


class MeshComponent : Component
{
public:
	MeshComponent(std::string mesh, std::string material = "default", std::string shader = "default")
	{
		if(ModelLibrary::GetInstance().MeshExists(mesh))
		{
			m_meshID = mesh;
		}
		else
		{
			ModelLibrary::GetInstance().AddMesh(mesh);
		}

		m_shaderID = shader;
		m_materialID;
	}

	std::string m_shaderID;
	std::string m_meshID;
	std::string m_materialID;
};