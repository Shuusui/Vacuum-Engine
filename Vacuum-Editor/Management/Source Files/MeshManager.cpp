#include "..\Header Files\MeshManager.h"
#include "Utilities\WaveFrontReader.h"
#include <fstream>

#define JSONMESHMAP "mesh_map"

Vacuum::CMeshManager* Vacuum::CMeshManager::s_meshManager = nullptr;

void Vacuum::CMeshManager::OnCreate(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath)
{
	if (s_meshManager)
	{
		delete s_meshManager;
		s_meshManager = nullptr;
	}
	s_meshManager = new CMeshManager(_meshesPath, _configsPath);
}

void Vacuum::CMeshManager::OnDestroy()
{
	if (s_meshManager)
	{
		delete s_meshManager;
		s_meshManager = nullptr;
	}
}

void Vacuum::CMeshManager::Load()
{

	if (std::filesystem::exists(m_configFilePath))
	{
		std::ifstream configFile(m_configFilePath);

		Json json = {};
		configFile >> json;

		Json meshMapJson = json[JSONMESHMAP].get<Json>();
		for (auto& [key, value] : meshMapJson.items())
		{
			m_meshes.insert(std::make_pair(key, value));
		}
	}

	for (const std::filesystem::path& meshPath : std::filesystem::directory_iterator(m_meshesPath))
	{
		std::filesystem::path extension = meshPath.extension();
		if (extension != ".obj" && extension != ".fbx")
		{
			continue;
		}
		m_meshes.insert(std::make_pair(meshPath.string(), SModel(meshPath.filename().string())));
	}
}

void Vacuum::CMeshManager::Save()
{
	Json json = {};
	Json meshMapJson = {}; 
	for (const auto& [key, value] : m_meshes)
	{
		meshMapJson[key] = value.ToJson();
	}
	json[JSONMESHMAP] = meshMapJson;

	std::ofstream configFile(m_configFilePath);

	configFile << json.dump();
}

Vacuum::CMeshManager::~CMeshManager()
{
	Save();
}

Vacuum::CMeshManager::CMeshManager(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath)
	:m_meshesPath(_meshesPath)
	,m_configsPath(_configsPath)
	,m_configFilePath(m_configsPath / "meshmanager.config")
{
	Load();
}
