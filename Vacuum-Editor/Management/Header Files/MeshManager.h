#pragma once
#include "Guid.h"
#include <filesystem>
#include <unordered_map>

namespace Vacuum
{
	class CMeshManager
	{
	public: 
		static void OnCreate(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath);
		static void OnDestroy();
		void Load();
		void Save();
		~CMeshManager();
	private:
		CMeshManager(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath);

		static CMeshManager* s_meshManager;
		std::filesystem::path m_meshesPath;
		std::filesystem::path m_configsPath;
		std::filesystem::path m_configFilePath;
		std::unordered_map<SGuid, std::filesystem::path> m_meshes;
	};
}