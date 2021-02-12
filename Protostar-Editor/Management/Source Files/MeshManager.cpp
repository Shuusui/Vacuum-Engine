#include "..\Header Files\MeshManager.h"
#include <fstream>
#include <unordered_set>

Protostar::CMeshManager* Protostar::CMeshManager::s_meshManager = nullptr;

void Protostar::CMeshManager::OnCreate(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath)
{
	if (s_meshManager)
	{
		delete s_meshManager;
		s_meshManager = nullptr;
	}
	s_meshManager = new CMeshManager(_meshesPath, _configsPath);
}

void Protostar::CMeshManager::OnDestroy()
{
	if (s_meshManager)
	{
		delete s_meshManager;
		s_meshManager = nullptr;
	}
}

std::filesystem::path Protostar::CMeshManager::GetModelPathAbsolute(const SModel& _model)
{
	return m_meshesPath / _model.Path;
}

void Protostar::CMeshManager::Load()
{
	WaveFrontReader<u32> wfReader = WaveFrontReader<u32>();
	std::unordered_set<std::string> alreadyFoundMeshes = {};
	using namespace JsonKeys;
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
			wfReader.Clear();
			std::filesystem::path modelAbsolutePath = GetModelPathAbsolute(model);
			if (!std::filesystem::exists(modelAbsolutePath))
			{
				continue;
			}
			wfReader.Load(modelAbsolutePath.wstring().c_str());
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
		
		SModel model = SModel(meshPath.lexically_relative(m_meshesPath));
		wfReader.Clear();
		wfReader.Load(meshPath.wstring().c_str());
		model.MeshData.Vertices = std::move(wfReader.vertices);
		model.MeshData.Indices = std::move(wfReader.indices);
		m_meshPaths.insert(model.Path.string());
		m_meshes.insert(std::make_pair(model.Guid, model));
	}
}

void Protostar::CMeshManager::Save()
{
	Json json = {};
	Json meshMapJson = {}; 

	using namespace JsonKeys;
	for (const auto& [key, value] : m_meshes)
	{
		meshMapJson[key.ToString()] = value.ToJson();
	}
	json[JSONMESHMAP] = meshMapJson;

	std::ofstream configFile(m_configFilePath, std::ios::trunc);

	configFile << json.dump();
}

Protostar::CMeshManager::~CMeshManager()
{
	Save();
}

Protostar::CMeshManager::CMeshManager(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath)
	:m_meshesPath(_meshesPath)
	,m_configsPath(_configsPath)
	,m_configFilePath(m_configsPath / "meshmanager.ini")
{
	Load();
}

#undef JSONMESHMAP