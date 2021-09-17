#include "..\Header Files\Project.h"
#include "..\Header Files\MeshManager.h"

#include "Json.h"
#include "ECS\Header Files\EntityManager.h"
#include "ShaderLibrary.h"
#include "RootSignatureLibrary.h"
#include "PSOLibrary.h"

import <fstream>;
import <string>;

const char* JSONNAME = "name";
const char* JSONGUID = "guid";
const char* JSONMOSTRECENTSCENEGUID = "most_recent_scene_guid";

Protostar::CProject::CProject(const std::filesystem::path& _projectPath)
	:m_guid(SGuid())
	,m_currentScene(nullptr)
	,m_name(std::string())
	,m_projectPaths(SProjectPaths())
{
	m_projectPaths.ProjectDir = _projectPath;
	m_projectPaths.ConfigDir = _projectPath / "Configs";
	m_projectPaths.ProjectFilePath = _projectPath / ".peproject";
	m_projectPaths.ContentDir = _projectPath / "Content";
	m_projectPaths.ShaderDir = m_projectPaths.ContentDir / "Shaders";
	m_projectPaths.ModelsDir = m_projectPaths.ContentDir / "Models";
	m_projectPaths.ScenesDir = m_projectPaths.ContentDir / "Scenes";
	m_projectPaths.EntitiesDir = m_projectPaths.ContentDir / "Entities";

	CShaderLibrary::Create(_projectPath);
	CRootSignatureLibrary::Create(_projectPath);
	CPSOLibrary::Create(_projectPath);

	std::ifstream projectFile(m_projectPaths.ProjectFilePath);
	Json json = {};
	projectFile >> json;
	m_name = json[JSONNAME].get<std::string>();
	m_guid = json[JSONGUID].get<std::string>();
	SGuid mostRecentSceneGuid = json[JSONMOSTRECENTSCENEGUID].get<std::string>();

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
		CScene* scene = new CScene(scenePath);
		m_scenes.insert(scene);
		if (scene->GetGuid() != mostRecentSceneGuid)
		{
			continue;
		}
		m_currentScene = scene;
	}

	for (const std::filesystem::path& entityPath : std::filesystem::directory_iterator(m_projectPaths.EntitiesDir))
	{
		CBaseEntity* entity = new CBaseEntity(entityPath);
	}

	CMeshManager::OnCreate(m_projectPaths.ModelsDir, m_projectPaths.ConfigDir);
}

Protostar::CProject::~CProject()
{
	Json json = {};
	json[JSONNAME] = m_name;
	json[JSONGUID] = m_guid.ToString();
	json[JSONMOSTRECENTSCENEGUID] = m_currentScene ? m_currentScene->GetGuid().ToString() :SGuid().ToString();

	std::ofstream projectFile(m_projectPaths.ProjectFilePath, std::ios::trunc);
	projectFile << json.dump();

	for (CScene* scene : m_scenes)
	{
		if (!scene)
		{
			continue;
		}
		delete scene;
		scene = nullptr;
	}
	CMeshManager::OnDestroy();
	CShaderLibrary::Destroy();
	CRootSignatureLibrary::Destroy();
}
