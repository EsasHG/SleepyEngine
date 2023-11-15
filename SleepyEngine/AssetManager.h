#pragma once

#include <fstream>

namespace Sleepy
{
	class Entity;
	class Scene;
	class AssetManager
	{
	public:
		AssetManager() {};
		
		static bool SaveScene(const Scene& scene);
		static Scene* LoadScene(std::string file);
	private:
		static bool SaveEntity(const Entity& entity, std::ofstream& file);
		static bool LoadEntity(std::string::iterator& it, std::string& text, Entity& entity);

	};
}

