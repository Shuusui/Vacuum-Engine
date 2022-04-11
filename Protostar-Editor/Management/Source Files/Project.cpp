#include "..\Header Files\Project.h"
#include "..\Header Files\MeshManager.h"

#include <fstream>
#include "Json.h"
#include <string>
#include "ECS\Header Files\EntityManager.h"
#include "ShaderLibrary.h"
#include "RootSignatureLibrary.h"
#include "PSOLibrary.h"

const char* JSONNAME = "name";
const char* JSONGUID = "guid";
const char* JSONMOSTRECENTSCENEGUID = "most_recent_scene_guid";

Protostar::PProject::PProject(const std::filesystem::path& _projectPath)
	:m_guid(PGuid())
	,m_currentScene(nullptr)
	,m_name(std::string())
	,m_projectPaths(PProjectPaths())
{
	m_projectPaths.ProjectDir = _projectPath;
	m_projectPaths.ConfigDir = _projectPath / "Configs";
	m_projectPaths.ProjectFilePath = _projectPath / ".peproject";
	m_projectPaths.ContentDir = _projectPath / "Content";
	m_projectPaths.ShaderDir = m_projectPaths.ContentDir / "Shaders";
	m_projectPaths.ModelsDir = m_projectPaths.ContentDir / "Models";
	m_projectPaths.ScenesDir = m_projectPaths.ContentDir / "Scenes";
	m_projectPaths.EntitiesDir = m_projectPaths.ContentDir / "Entities";

	PShaderLibrary::Create(_projectPath);
	PRootSignatureLibrary::Create(_projectPath);
	PPSOLibrary::Create(_projectPath);

	std::ifstream projectFile(m_projectPaths.ProjectFilePath);
	PJson json = {};
	projectFile >> json;
	m_name = json[JSONNAME].get<std::string>();
	m_guid = json[JSONGUID].get<std::string>();
	PGuid mostRecentSceneGuid = json[JSONMOSTRECENTSCENEGUID].get<std::string>();

	if (!std::filesystem::exists(m_projectPaths.ScenesDir))
	{
		std::filesystem::create_directories(m_projectPaths.ScenesDir);
	}

	if (!std::filesystem::exists(m_projectPaths.EntitiesDir))
	{
		std::filesystem::create_directories(m_projectPaths.EntitiesDir);
	}

	for (const std::filesystem::path& scenePath : std::filesystem::directory_iterator(m_projectPaths.ScenesDir))
	{
		PScene* scene = new PScene(scenePath);
		m_scenes.insert(scene);
		if (scene->GetGuid() != mostRecentSceneGuid)
		{
			continue;
		}
		m_currentScene = scene;
	}

	for (const std::filesystem::path& entityPath : std::filesystem::directory_iterator(m_projectPaths.EntitiesDir))
	{
		PBaseEntity* entity = new PBaseEntity(entityPath);
	}

	PMeshManager::OnCreate(m_projectPaths.ModelsDir, m_projectPaths.ConfigDir);
}

Protostar::PProject::~PProject()
{
	PJson json = {};
	json[JSONNAME] = m_name;
	json[JSONGUID] = m_guid.ToString();
	json[JSONMOSTRECENTSCENEGUID] = m_currentScene ? m_currentScene->GetGuid().ToString() :PGuid().ToString();

	std::ofstream projectFile(m_projectPaths.ProjectFilePath, std::ios::trunc);
	projectFile << json.dump();

	for (PScene* scene : m_scenes)
	{
		if (!scene)
		{
			continue;
		}
		delete scene;
		scene = nullptr;
	}
	PMeshManager::OnDestroy();
	PShaderLibrary::Destroy();
	PRootSignatureLibrary::Destroy();
}
