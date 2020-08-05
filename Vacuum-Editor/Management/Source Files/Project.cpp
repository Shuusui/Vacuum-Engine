#include "..\Header Files\Project.h"

#include <fstream>
#include "Json.h"

Vacuum::CProject::CProject(const std::filesystem::path& _projectPath)
{
	m_currentProjectDir = _projectPath;
	m_currentContentDir = _projectPath;
	m_configsDir = _projectPath / "Configs";
	m_projectConfigPath = m_configsDir / "project.config";
	m_currentShaderDir = m_currentContentDir.append("Content");
	m_currentShaderDir.append("Shaders");

	std::ifstream projectConfig(m_projectConfigPath);
	Json json = {};
	projectConfig >> json;
	m_name = json["name"].get<std::wstring>();
	m_guid = json["guid"].get<std::wstring>();
}