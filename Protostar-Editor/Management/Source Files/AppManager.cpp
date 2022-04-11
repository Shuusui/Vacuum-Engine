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


Protostar::PAppManager* Protostar::PAppManager::s_app = nullptr;

const char* JSONHEIGHT = "height";
const char* JSONWIDTH = "width";
const char* JSONX = "x";
const char* JSONY = "y";
const char* JSONCURRPROJGUID = "current_project_guid";
const char* JSONVSYNC = "vsync";

bool Protostar::PAppManager::InitApp(const HINSTANCE& _hInstance)
{
	if (!s_app)
	{
		s_app = new PAppManager();
	}

	PE_LOG("Initialize app");

	std::filesystem::path currentPath = std::filesystem::current_path();
	std::wstring::size_type pos = std::wstring(currentPath.c_str()).find_last_of(L"\\/");
	s_app->m_appPaths.RootDir = std::wstring(currentPath).substr(0, pos);
	std::filesystem::current_path(s_app->m_appPaths.RootDir);
	s_app->m_appPaths.ConfigDir = s_app->m_appPaths.RootDir / L"Config";
	if (!std::filesystem::exists(s_app->m_appPaths.ConfigDir))
	{
		PE_LOG("Creating config directory");
		std::filesystem::create_directory(s_app->m_appPaths.ConfigDir);
	}

	s_app->m_appPaths.ProjectsDir = s_app->m_appPaths.RootDir / L"Projects";
	s_app->m_appConfigPath = s_app->m_appPaths.ConfigDir / L"app.config";

	PGuid projectGuid = {};
	if (std::filesystem::exists(s_app->m_appConfigPath))
	{
		PJson json = {};
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

	std::string errorMsg = {};

	PWindowInfo windowInfo = {};
	windowInfo.ClassParams.ClassName = TEXT("Protostar Engine window");
	windowInfo.ClassParams.HInstance = _hInstance;
	windowInfo.ClassParams.BackgroundColor = CreateSolidBrush(RGB(1, 1, 1));
	windowInfo.ClassParams.Style = CS_HREDRAW | CS_VREDRAW;
	windowInfo.CreationParams.DwExStyle = NULL;
	windowInfo.CreationParams.DwStyle = WS_OVERLAPPEDWINDOW;
	windowInfo.CreationParams.WindowName = TEXT("Protostar-Engine");
	windowInfo.CreationParams.ParentWindow = nullptr;
	windowInfo.CreationParams.Menu = nullptr;
	windowInfo.CreationParams.LpParam = nullptr;
	windowInfo.DimParams = s_app->m_mainWindowDim;

	PMainWindow::InitWindow(windowInfo);
	if (!PMainWindow::Create(errorMsg))
	{
		PE_LOG(errorMsg.c_str());
		return false;
	}

	PRendererManager::Create(SRendererCreationInfo{ ERenderAPIs::DX12, (u32)s_app->m_mainWindowDim.Width, (u32)s_app->m_mainWindowDim.Height, s_app->m_bLastVSyncState, reinterpret_cast<void*>(PMainWindow::GetWindowHandle()->GetHwnd()) });

	s_app->LoadProjects();
	if (projectGuid.IsValid())
	{
		s_app->LoadRecentProject(projectGuid);
	}
	return true;
}

void Protostar::PAppManager::Destroy()
{
	PMainWindow* mainWindow = PMainWindow::GetWindowHandle();
	if (mainWindow)
	{
		s_app->m_mainWindowDim = mainWindow->GetCurrentDim();
	}

	if (!std::filesystem::exists(s_app->m_appPaths.ConfigDir))
	{
		std::filesystem::create_directory(s_app->m_appPaths.ConfigDir);
	}

	SWindowDimParams wndDim = s_app->m_mainWindowDim;
	PJson json = 
	{
		{JSONHEIGHT, wndDim.Height}, 
		{JSONWIDTH, wndDim.Width},
		{JSONX, wndDim.LeftTopCornerX}, 
		{JSONY, wndDim.LeftTopCornerY},
		{JSONCURRPROJGUID, s_app->m_currentProject ? s_app->m_currentProject->GetGuid().ToString() : PGuid().ToString()},
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

Protostar::PAppManager* Protostar::PAppManager::GetAppHandle()
{
	return s_app;
}

Protostar::SWindowDimParams Protostar::PAppManager::GetInitWindowDimParams()
{
	return s_app->m_mainWindowDim;
}

Protostar::PAppPaths Protostar::PAppManager::GetAppPaths()
{
	return s_app->m_appPaths;
}

void Protostar::PAppManager::LoadProject(PProject* _project)
{
	if (!_project)
	{
		return;
	}
	PProject* oldProject = m_currentProject;
	if (oldProject)
	{
		PE_LOG_F("Unload project: %s", oldProject->GetName().c_str());
	}
	m_currentProject = _project;
	for (const std::function<void(PProject*, PProject*)>& func : m_registeredOnLoadProjectCallbacks)
	{
		func(oldProject, _project);
	}

	PE_LOG_F("Load project: %s", _project->GetName().c_str());
}

void Protostar::PAppManager::LoadRecentProject(const PGuid& _projectGuid)
{
	for (PProject* project : m_projects)
	{
		if (project->GetGuid() == _projectGuid)
		{
			PE_LOG_F("Load most recent project: %s", project->GetName().c_str());
			LoadProject(project);
			break;
		}
	}
}

void Protostar::PAppManager::LoadProjects()
{
	for (const std::filesystem::path& projectPath : std::filesystem::directory_iterator(m_appPaths.ProjectsDir))
	{
		PProject* project = new PProject(projectPath);
		m_projects.push_back(project);
		PE_LOG_F("Found project: %s", project->GetName().c_str());
	}
}

Protostar::PAppManager::~PAppManager()
{
	for (PProject* project : m_projects)
	{
		if (!project)
		{
			continue;
		}
		delete project;
		project = nullptr;
	}
}

Protostar::PAppManager::PAppManager()
	:m_mainWindowDim(SWindowDimParams())
	,m_appPaths(PAppPaths())
	,m_appConfigPath(std::filesystem::path())
	,m_currentProject(nullptr)
	,m_bLastVSyncState(false)
{
}
