#include "..\Header Files\Project.h"

Vacuum::CProject::CProject(const std::filesystem::path& _projectPath)
	:m_guid(SGuid::NewGuid())
{
	m_currentProjectDir = _projectPath;
	m_currentContentDir = _projectPath;
	m_currentShaderDir = m_currentContentDir.append("Content");
	m_currentShaderDir.append("Shader");
}

Vacuum::CProject::CProject(const std::filesystem::path& _projectPath, const SGuid& _projectGuid)
	:m_guid(_projectGuid)
{
	m_currentProjectDir = _projectPath;
	m_currentContentDir = _projectPath;
	m_currentShaderDir = m_currentContentDir.append("Content");
	m_currentShaderDir.append("Shader");
}
