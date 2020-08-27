#pragma once
#include "Guid.h"
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include "Json.h"
#include "Utilities\WaveFrontReader.h"
#include "SharedStructs.h"

#define JSONMESHGUID "guid"
#define JSONMESHNAME "name"
#define JSONMESHPATH "path"

namespace Vacuum
{
	struct SModel
	{
		SModel()
			:Guid(SGuid())
			,Name(std::string())
			,Path(std::filesystem::path())
			,MeshData(SMesh())
		{

		}

		SModel(const std::filesystem::path& _path)
			:Guid(SGuid::NewGuid())
			,Name(_path.filename().string())
			,Path(_path)
		{
		}

		SModel(const Json& _json)
		{
			Guid = _json[JSONMESHGUID].get<std::string>();
			Name = _json[JSONMESHNAME].get<std::string>();
			Path = _json[JSONMESHPATH].get<std::string>();
		}

		SModel(const SModel& _other)
			:Guid(_other.Guid)
			,Name(_other.Name)
			,Path(_other.Path)
			,MeshData(_other.MeshData)
		{

		}

		Json ToJson() const
		{
			return Json{
				{JSONMESHGUID, Guid.ToString()},
				{JSONMESHNAME, Name},
				{JSONMESHPATH, Path.string()}
			};
		}

		SGuid Guid;
		std::string Name;
		std::filesystem::path Path;
		SMesh MeshData;
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

		std::unordered_map<SGuid, SModel> GetMeshes() const
		{
			return m_meshes;
		}

		std::unordered_set<std::string> GetMeshPaths() const
		{
			return m_meshPaths;
		}

		SModel GetModelData(const SGuid& _guid)
		{
			return m_meshes[_guid];
		}

		void RegisterModel(const std::filesystem::path& _modelPath)
		{
			m_meshes.insert(std::make_pair(SGuid::NewGuid(), SModel(_modelPath)));
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
		std::unordered_map<SGuid, SModel> m_meshes;
		std::unordered_set<std::string> m_meshPaths;
	};
}

#undef JSONMESHGUID
#undef JSONMESHNAME
#undef JSONMESHPATH