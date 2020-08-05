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
	s_app->m_appPaths.m_rootDir = std::wstring(currentPath).substr(0, pos);
	std::filesystem::current_path(s_app->m_appPaths.m_rootDir);
	s_app->m_appPaths.m_configDir = s_app->m_appPaths.m_rootDir / L"Config";
	if (!std::filesystem::exists(s_app->m_appPaths.m_configDir))
	{
		VE_LOG(TEXT("Creating config directory"));
		std::filesystem::create_directory(s_app->m_appPaths.m_configDir);
	}

	s_app->m_appPaths.m_projectsDir = s_app->m_appPaths.m_rootDir / L"Projects";
	s_app->m_appConfigPath = s_app->m_appPaths.m_configDir / L"app.config";

	if (std::filesystem::exists(s_app->m_appConfigPath))
	{
		Json json = {};
		std::ifstream appConfig(s_app->m_appConfigPath);
		appConfig >> json;
		s_app->m_mainWindowDim.m_height = json["height"].get<int64>();
		s_app->m_mainWindowDim.m_width = json["width"].get<int64>();
		s_app->m_mainWindowDim.m_leftTopCornerX = json["x"].get<int32>();
		s_app->m_mainWindowDim.m_leftTopCornerY = json["y"].get<int32>();
	}
	else
	{
		s_app->m_mainWindowDim.m_height = 720;
		s_app->m_mainWindowDim.m_width = 1280;
		s_app->m_mainWindowDim.m_leftTopCornerX = 0;
		s_app->m_mainWindowDim.m_leftTopCornerY = 0;
	}
}

void Vacuum::CAppManager::Destroy()
{
	CMainWindow* mainWindow = CMainWindow::GetWindowHandle();
	if (mainWindow)
	{
		s_app->m_mainWindowDim = mainWindow->GetCurrentDim();
	}

	if (!std::filesystem::exists(s_app->m_appPaths.m_configDir))
	{
		std::filesystem::create_directory(s_app->m_appPaths.m_configDir);
	}
	Json json = {};
	json["height"] = s_app->m_mainWindowDim.m_height;
	json["width"] = s_app->m_mainWindowDim.m_width;
	json["x"] = s_app->m_mainWindowDim.m_leftTopCornerX;
	json["y"] = s_app->m_mainWindowDim.m_leftTopCornerY;
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

Vacuum::CAppManager::CAppManager()
	:m_mainWindowDim(SWindowDimParams())
{
}
