#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Guid.h"
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include "Json.h"
#include "SharedStructs.h"

namespace Protostar
{
	struct PModel
	{
		PModel()
			:Guid(PGuid())
			,Name(std::string())
			,Path(std::filesystem::path())
			,MeshData(SMesh())
		{

		}

		PModel(const std::filesystem::path& _path)
			:Guid(PGuid::NewGuid())
			,Name(_path.filename().string())
			,Path(_path)
		{
		}


		PModel(const PModel&) = default;

		PModel& operator=(const PModel&) = default;


		PGuid Guid;
		std::string Name;
		std::filesystem::path Path;
		SMesh MeshData;
	};

	class PMeshManager
	{
	public: 
		static void OnCreate(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath);
		static void OnDestroy();
		static PMeshManager* GetHandle()
		{
			return s_meshManager;
		}

		std::unordered_map<PGuid, PModel> GetMeshes() const
		{
			return m_meshes;
		}

		std::unordered_set<std::string> GetMeshPaths() const
		{
			return m_meshPaths;
		}

		PModel GetModelData(const PGuid& _guid)
		{
			return m_meshes[_guid];
		}

		void RegisterModel(const std::filesystem::path& _modelPath)
		{
			m_meshes.insert(std::make_pair(PGuid::NewGuid(), PModel(_modelPath)));
			m_meshPaths.insert(_modelPath.string());
		}

		void UnregisterModel(const PGuid& _guid, const std::string& _modelPath)
		{
			m_meshes.erase(_guid);
			m_meshPaths.erase(_modelPath);
		}

		void Load();
		void Save();
		~PMeshManager();
	private:
		PMeshManager(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath);

		static PMeshManager* s_meshManager;
		std::filesystem::path m_meshesPath;
		std::filesystem::path m_configsPath;
		std::filesystem::path m_configFilePath;
		std::unordered_map<PGuid, PModel> m_meshes;
		std::unordered_set<std::string> m_meshPaths;
	};
}

#endif //MESHMANAGER_H