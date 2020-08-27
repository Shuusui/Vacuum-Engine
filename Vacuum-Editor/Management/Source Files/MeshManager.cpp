#include "..\Header Files\MeshManager.h"
#include <fstream>
#include <unordered_set>

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
	std::unordered_set<std::string> alreadyFoundMeshes = {};
	if (std::filesystem::exists(m_configFilePath))
	{
		std::ifstream configFile(m_configFilePath);

		Json json = {};
		configFile >> json;

		Json meshMapJson = json[JSONMESHMAP].get<Json>();
		for (auto& [key, value] : meshMapJson.items())
		{
			SModel model = SModel(value);
			alreadyFoundMeshes.insert(model.Name);
			m_meshes.insert(std::make_pair(key, model));
			m_meshPaths.insert(model.Path.string());
		}
	}

	WaveFrontReader<uint32> wfReader = WaveFrontReader<uint32>();
	for (const std::filesystem::path& meshPath : std::filesystem::directory_iterator(m_meshesPath))
	{
		if (alreadyFoundMeshes.find(meshPath.filename().string()) != alreadyFoundMeshes.end())
		{
			continue;
		}

		std::filesystem::path extension = meshPath.extension();
		if (extension != ".obj" && extension != ".fbx")
		{
			continue;
		}
		SModel model = SModel(meshPath);
		wfReader.Clear();
		wfReader.Load(meshPath.wstring().c_str());
		model.MeshData.Vertices = std::move(wfReader.vertices);
		model.MeshData.Indices = std::move(wfReader.indices);
		m_meshPaths.insert(model.Path.string());
		m_meshes.insert(std::make_pair(model.Guid, model));
	}
}

void Vacuum::CMeshManager::Save()
{
	Json json = {};
	Json meshMapJson = {}; 
	for (const auto& [key, value] : m_meshes)
	{
		meshMapJson[key.ToString()] = value.ToJson();
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
