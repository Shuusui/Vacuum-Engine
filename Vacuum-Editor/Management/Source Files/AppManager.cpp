#include "..\Header Files\AppManager.h"

#pragma region Internal Includes 
#include "Json.h"
#include "Window.h"
#include "Log.h"
#pragma endregion //Internal Includes

#pragma region External Includes
#include <fstream>
#include <windows.h>
#pragma endregion //External Includes


Vacuum::CAppManager* Vacuum::CAppManager::s_app = nullptr;


void Vacuum::CAppManager::InitApp()
{
	if (!s_app)
	{
		s_app = new CAppManager();
	}

	VE_LOG(TEXT("Initialize app"));

	std::filesystem::path currentPath = std::filesystem::current_path();
	std::wstring::size_type pos = std::wstring(currentPath.c_str()).find_last_of(L"\\/");
	s_app->m_appPaths.RootDir = std::wstring(currentPath).substr(0, pos);
	std::filesystem::current_path(s_app->m_appPaths.RootDir);
	s_app->m_appPaths.ConfigDir = s_app->m_appPaths.RootDir / L"Config";
	if (!std::filesystem::exists(s_app->m_appPaths.ConfigDir))
	{
		VE_LOG(TEXT("Creating config directory"));
		std::filesystem::create_directory(s_app->m_appPaths.ConfigDir);
	}

	s_app->m_appPaths.ProjectsDir = s_app->m_appPaths.RootDir / L"Projects";
	s_app->m_appConfigPath = s_app->m_appPaths.ConfigDir / L"app.config";

	SGuid projectGuid = {};
	if (std::filesystem::exists(s_app->m_appConfigPath))
	{
		Json json = {};
		std::ifstream appConfig(s_app->m_appConfigPath);
		appConfig >> json;
		s_app->m_mainWindowDim.Height = json["height"].get<int64>();
		s_app->m_mainWindowDim.Width = json["width"].get<int64>();
		s_app->m_mainWindowDim.LeftTopCornerX = json["x"].get<int32>();
		s_app->m_mainWindowDim.LeftTopCornerY = json["y"].get<int32>();
		projectGuid = json["current_project_guid"].get<std::wstring>();
	}
	else
	{
		s_app->m_mainWindowDim.Height = 720;
		s_app->m_mainWindowDim.Width = 1280;
		s_app->m_mainWindowDim.LeftTopCornerX = 0;
		s_app->m_mainWindowDim.LeftTopCornerY = 0;
	}
	s_app->LoadProjects();
	if (projectGuid.IsValid())
	{
		s_app->LoadRecentProject(projectGuid);
	}
}

void Vacuum::CAppManager::Destroy()
{
	CMainWindow* mainWindow = CMainWindow::GetWindowHandle();
	if (mainWindow)
	{
		s_app->m_mainWindowDim = mainWindow->GetCurrentDim();
	}

	if (!std::filesystem::exists(s_app->m_appPaths.ConfigDir))
	{
		std::filesystem::create_directory(s_app->m_appPaths.ConfigDir);
	}
	Json json = {};
	json["height"] = s_app->m_mainWindowDim.Height;
	json["width"] = s_app->m_mainWindowDim.Width;
	json["x"] = s_app->m_mainWindowDim.LeftTopCornerX;
	json["y"] = s_app->m_mainWindowDim.LeftTopCornerY;
	json["current_project_guid"] = s_app->m_currentProject ? s_app->m_currentProject->GetGuid().ToString() : SGuid().ToString();
	std::ofstream appConfig(s_app->m_appConfigPath, std::ios::trunc);
	appConfig << json.dump();

	if (s_app)
	{
		delete s_app;
		s_app = nullptr;
	}
}

Vacuum::CAppManager* Vacuum::CAppManager::GetAppHandle()
{
	return s_app;
}

Vacuum::SWindowDimParams Vacuum::CAppManager::GetInitWindowDimParams()
{
	return s_app->m_mainWindowDim;
}

Vacuum::SAppPaths Vacuum::CAppManager::GetAppPaths()
{
	return s_app->m_appPaths;
}

void Vacuum::CAppManager::LoadProject(CProject* _project)
{
	if (!_project)
	{
		return;
	}
	CProject* oldProject = m_currentProject;
	if (oldProject)
	{
		VE_LOG_F("Unload project: %s", oldProject->GetName().c_str());
	}
	m_currentProject = _project;
	for (const std::function<void(CProject*, CProject*)>& func : m_registeredOnLoadProjectCallbacks)
	{
		func(oldProject, _project);
	}

	VE_LOG_F("Load project: %s", _project->GetName().c_str());
}

void Vacuum::CAppManager::LoadRecentProject(const SGuid& _projectGuid)
{
	for (CProject* project : m_projects)
	{
		if (project->GetGuid() == _projectGuid)
		{
			VE_LOG_F("Load most recent project: %s", project->GetName().c_str());
			LoadProject(project);
			break;
		}
	}
}

void Vacuum::CAppManager::LoadProjects()
{
	for (const std::filesystem::path& projectPath : std::filesystem::directory_iterator(m_appPaths.ProjectsDir))
	{
		CProject* project = new CProject(projectPath);
		m_projects.push_back(project);
		VE_LOG_F("Found project: %s", project->GetName().c_str());
	}
}

Vacuum::CAppManager::~CAppManager()
{
	for (CProject* project : m_projects)
	{
		if (!project)
		{
			continue;
		}
		delete project;
		project = nullptr;
	}
}

Vacuum::CAppManager::CAppManager()
	:m_mainWindowDim(SWindowDimParams())
	,m_appPaths(SAppPaths())
	,m_appConfigPath(std::filesystem::path())
	,m_currentProject(nullptr)
{
}
