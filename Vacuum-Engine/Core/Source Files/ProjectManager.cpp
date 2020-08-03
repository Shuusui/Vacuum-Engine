#include "..\Header Files\Public\ProjectManager.h"
#include "Json.h"

Vacuum::CProjectManager* Vacuum::CProjectManager::s_projectManager = nullptr;

void Vacuum::CProjectManager::Create(const std::wstring& _rootDir)
{
	if (s_projectManager)
	{
		return;
	}
	s_projectManager = new CProjectManager(_rootDir);
}

void Vacuum::CProjectManager::OpenProject(const SGuid& _projectGuid)
{

}

void Vacuum::CProjectManager::CreateNewProject(const std::wstring& _projectName)
{

}

void Vacuum::CProjectManager::Destroy()
{
	if (s_projectManager)
	{
		delete s_projectManager;
		s_projectManager = nullptr;
	}
}

Vacuum::CProjectManager::CProjectManager(const std::wstring& _rootDir)
	:m_projectsDir(std::filesystem::path(_rootDir) / "Projects")
	,m_currentProject(nullptr)
{

}
