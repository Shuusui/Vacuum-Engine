#include "..\Header Files\Project.h"

#include <fstream>
#include "Json.h"
#include <string>

Vacuum::CProject::CProject(const std::filesystem::path& _projectPath)
	:m_guid(SGuid())
	,m_mostRecentSceneGuid(SGuid())
	,m_name(std::string())
	,m_projectPaths(SProjectPaths())
{
	m_projectPaths.ProjectDir = _projectPath;
	m_projectPaths.ContentDir = _projectPath;
	m_projectPaths.ConfigDir = _projectPath / "Configs";
	m_projectPaths.ProjectFilePath = _projectPath / ".project";
	m_projectPaths.ContentDir = _projectPath / "Content";
	m_projectPaths.ShaderDir = m_projectPaths.ContentDir / "Shaders";
	m_projectPaths.ModelsDir = m_projectPaths.ContentDir / "Models";

	std::ifstream projectFile(m_projectPaths.ProjectFilePath);
	Json json = {};
	projectFile >> json;
	m_name = json["name"].get<std::string>();
	m_guid = json["guid"].get<std::wstring>();
}

Vacuum::CProject::~CProject()
{
	Json json = {};
	json["name"] = m_name;
	json["guid"] = m_guid.ToString();
	json["most_recent_scene_guid"] = m_mostRecentSceneGuid.ToString();

	std::ofstream projectFile(m_projectPaths.ProjectFilePath, std::ios::trunc);
	projectFile << json.dump();
}
