#include "..\Header Files\Project.h"

#include <fstream>
#include "Json.h"

Vacuum::CProject::CProject(const std::filesystem::path& _projectPath)
{
	m_currentProjectDir = _projectPath;
	m_currentContentDir = _projectPath;
	m_configsDir = _projectPath / "Configs";
	m_projectFile = _projectPath / ".project";
	m_currentShaderDir = m_currentContentDir.append("Content");
	m_currentShaderDir.append("Shaders");
	m_modelDir = m_currentContentDir / "Models";

	std::ifstream projectFile(m_projectFile);
	Json json = {};
	projectFile >> json;
	m_name = json["name"].get<std::string>();
	m_guid = json["guid"].get<std::wstring>();

	for (const std::filesystem::path& shader : std::filesystem::recursive_directory_iterator(m_currentShaderDir))
	{
		if (std::filesystem::is_directory(shader) || shader.extension() != ".hlsl")
		{
			continue;
		}
		m_shaderPaths.push_back(shader);
	}
	for (const std::filesystem::path& model : std::filesystem::recursive_directory_iterator(m_modelDir))
	{
		if (std::filesystem::is_directory(model) || model.extension() != ".obj")
		{
			continue;
		}
		m_modelPaths.push_back(model);
	}
}