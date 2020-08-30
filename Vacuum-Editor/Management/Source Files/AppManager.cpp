#include "..\Header Files\AppManager.h"

#pragma region Internal Includes 
#include "Json.h"
#include "Window.h"
#include "Log.h"
#include "RendererManager.h"
#pragma endregion //Internal Includes

#pragma region External Includes
#include <fstream>
#include <windows.h>
#pragma endregion //External Includes


Vacuum::CAppManager* Vacuum::CAppManager::s_app = nullptr;

const char* JSONHEIGHT = "height";
const char* JSONWIDTH = "width";
const char* JSONX = "x";
const char* JSONY = "y";
const char* JSONCURRPROJGUID = "current_project_guid";
const char* JSONVSYNC = "vsync";

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
		if (json.contains(JSONHEIGHT))
		{
			s_app->m_mainWindowDim.Height = json[JSONHEIGHT].get<s64>();
		}
		if (json.contains(JSONWIDTH))
		{
			s_app->m_mainWindowDim.Width = json[JSONWIDTH].get<s64>();
		}
		if (json.contains(JSONX))
		{
			s_app->m_mainWindowDim.LeftTopCornerX = json[JSONX].get<s32>();
		}
		if (json.contains(JSONY))
		{
			s_app->m_mainWindowDim.LeftTopCornerY = json[JSONY].get<s32>();
		}
		if (json.contains(JSONVSYNC))
		{
			s_app->m_bLastVSyncState = json[JSONVSYNC].get<bool>();
		}
		if (json.contains(JSONCURRPROJGUID))
		{
			projectGuid = json[JSONCURRPROJGUID].get<std::string>();
		}
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

	SWindowDimParams wndDim = s_app->m_mainWindowDim;
	Json json = 
	{
		{JSONHEIGHT, wndDim.Height}, 
		{JSONWIDTH, wndDim.Width},
		{JSONX, wndDim.LeftTopCornerX}, 
		{JSONY, wndDim.LeftTopCornerY},
		{JSONCURRPROJGUID, s_app->m_currentProject ? s_app->m_currentProject->GetGuid().ToString() : SGuid().ToString()},
		{JSONVSYNC, s_app->m_bLastVSyncState}
	};

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
	,m_bLastVSyncState(false)
{
}
