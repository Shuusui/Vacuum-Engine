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

	if (std::filesystem::exists(s_app->m_appConfigPath))
	{
		Json json = {};
		std::ifstream appConfig(s_app->m_appConfigPath);
		appConfig >> json;
		s_app->m_mainWindowDim.Height = json["height"].get<int64>();
		s_app->m_mainWindowDim.Width = json["width"].get<int64>();
		s_app->m_mainWindowDim.LeftTopCornerX = json["x"].get<int32>();
		s_app->m_mainWindowDim.LeftTopCornerY = json["y"].get<int32>();
	}
	else
	{
		s_app->m_mainWindowDim.Height = 720;
		s_app->m_mainWindowDim.Width = 1280;
		s_app->m_mainWindowDim.LeftTopCornerX = 0;
		s_app->m_mainWindowDim.LeftTopCornerY = 0;
	}

	s_app->LoadProject();
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
	std::ofstream appConfig(s_app->m_appConfigPath, std::ios::trunc);
	appConfig << json.dump();
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

void Vacuum::CAppManager::LoadProject()
{
	for (const std::filesystem::path& project : std::filesystem::directory_iterator(m_appPaths.ProjectsDir))
	{
		m_currentProject = new CProject(project);
	}
}

Vacuum::CAppManager::CAppManager()
	:m_mainWindowDim(SWindowDimParams())
	,m_appPaths(SAppPaths())
	,m_appConfigPath(std::filesystem::path())
	,m_currentProject(nullptr)
{
}
