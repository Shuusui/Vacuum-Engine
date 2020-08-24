#pragma once
#include "Guid.h"
#include <filesystem>
#include <unordered_map>
#include "Json.h"

#define JSONMESHGUID "guid"
#define JSONMESHNAME "name"

namespace Vacuum
{
	struct SModel
	{
		SModel(const std::string& _name)
			:Guid(SGuid::NewGuid())
			,Name(_name)
		{

		}

		SModel(const Json& _json)
		{
			Guid = _json[JSONMESHGUID].get<std::string>();
			Name = _json[JSONMESHNAME].get<std::string>();
		}

		Json ToJson() const
		{
			return Json{
				{JSONMESHGUID, Guid.ToString()},
				{JSONMESHNAME, Name}
			};
		}

		SGuid Guid;
		std::string Name;
	};

	class CMeshManager
	{
	public: 
		static void OnCreate(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath);
		static void OnDestroy();
		static CMeshManager* GetHandle()
		{
			return s_meshManager;
		}
		std::unordered_map<std::string, SModel> GetMeshes() const
		{
			return m_meshes;
		}

		void RegisterModel(const std::filesystem::path& _modelPath)
		{
			m_meshes.insert(std::make_pair(_modelPath.string(), SModel(_modelPath.filename().string())));
		}
		void Load();
		void Save();
		~CMeshManager();
	private:
		CMeshManager(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath);

		static CMeshManager* s_meshManager;
		std::filesystem::path m_meshesPath;
		std::filesystem::path m_configsPath;
		std::filesystem::path m_configFilePath;
		std::unordered_map<std::string, SModel> m_meshes;
	};
}

#undef JSONMESHGUID
#undef JSONMESHNAME