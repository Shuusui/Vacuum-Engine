#include "..\Header Files\MeshManager.h"
#include <fstream>
#include <unordered_set>

#definePJsonMESHMAP "mesh_map"

Protostar::PMeshManager* Protostar::PMeshManager::s_meshManager = nullptr;

void Protostar::PMeshManager::OnCreate(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath)
{
	if (s_meshManager)
	{
		delete s_meshManager;
		s_meshManager = nullptr;
	}
	s_meshManager = new PMeshManager(_meshesPath, _configsPath);
}

void Protostar::PMeshManager::OnDestroy()
{
	if (s_meshManager)
	{
		delete s_meshManager;
		s_meshManager = nullptr;
	}
}

void Protostar::PMeshManager::Load()
{
	WaveFrontReader<u32> wfReader = WaveFrontReader<u32>();
	std::unordered_set<std::string> alreadyFoundMeshes = {};
	if (std::filesystem::exists(m_configFilePath))
	{
		std::ifstream configFile(m_configFilePath);

		PJsonPJson = {};
		configFile >>PJson;

		PJson meshMapJson =PJson[JSONMESHMAP].get<PJson>();
		for (auto& [key, value] : meshMapJson.items())
		{
			PModel model = PModel(value);
			alreadyFoundMeshes.insert(model.Name);
			wfReader.Clear();
			wfReader.Load(model.Path.wstring().c_str());
			model.MeshData.Vertices = std::move(wfReader.vertices);
			model.MeshData.Indices = std::move(wfReader.indices);

			m_meshes.insert(std::make_pair(key, model));
			m_meshPaths.insert(model.Path.string());

		}
	}

	if(!std::filesystem::exists(m_meshesPath))
	{
		std::filesystem::create_directory(m_meshesPath);
	}

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
		PModel model = PModel(meshPath);
		wfReader.Clear();
		wfReader.Load(meshPath.wstring().c_str());
		model.MeshData.Vertices = std::move(wfReader.vertices);
		model.MeshData.Indices = std::move(wfReader.indices);
		m_meshPaths.insert(model.Path.string());
		m_meshes.insert(std::make_pair(model.Guid, model));
	}
}

void Protostar::PMeshManager::Save()
{
	PJsonPJson = {};
	PJson meshMapJson = {}; 

	for (const auto& [key, value] : m_meshes)
	{
		meshMapJson[key.ToString()] = value.ToJson();
	}
	json[JSONMESHMAP] = meshMapJson;

	std::ofstream configFile(m_configFilePath, std::ios::trunc);

	configFile <<PJson.dump();
}

Protostar::PMeshManager::~PMeshManager()
{
	Save();
}

Protostar::PMeshManager::PMeshManager(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath)
	:m_meshesPath(_meshesPath)
	,m_configsPath(_configsPath)
	,m_configFilePath(m_configsPath / "meshmanager.ini")
{
	Load();
}

#undefPJsonMESHMAP