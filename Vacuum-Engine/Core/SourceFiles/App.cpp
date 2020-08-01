#include "..\HeaderFiles\Public\App.h"

#pragma region Internal Includes 
#include "..\HeaderFiles\Public\Json.h"
#include "..\HeaderFiles\Public\Window.h"
#pragma endregion //Internal Includes

#pragma region External Includes
#include <fstream>
#include <windows.h>
#pragma endregion //External Includes


Vacuum::CApp* Vacuum::CApp::s_app = nullptr;


void Vacuum::CApp::InitApp()
{
	if (!s_app)
	{
		s_app = new CApp();
	}
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::wstring::size_type pos = std::wstring(currentPath.c_str()).find_last_of(L"\\/");
	s_app->m_rootDir = std::wstring(currentPath).substr(0, pos);
	std::filesystem::current_path(s_app->m_rootDir);
	s_app->m_configDir = s_app->m_rootDir.append(L"Config");
	if (!std::filesystem::exists(s_app->m_configDir))
	{
		std::filesystem::create_directory(s_app->m_configDir);
	}
	s_app->m_appConfigPath = s_app->m_configDir;
	s_app->m_appConfigPath.append(L"app.config");
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

void Vacuum::CApp::Destroy()
{
	CMainWindow* mainWindow = CMainWindow::GetWindowHandle();
	if (mainWindow)
	{
		s_app->m_mainWindowDim = mainWindow->GetCurrentDim();
	}

	if (!std::filesystem::exists(s_app->m_configDir))
	{
		std::filesystem::create_directory(s_app->m_configDir);
	}
	Json json = {};
	json["height"] = s_app->m_mainWindowDim.m_height;
	json["width"] = s_app->m_mainWindowDim.m_width;
	json["x"] = s_app->m_mainWindowDim.m_leftTopCornerX;
	json["y"] = s_app->m_mainWindowDim.m_leftTopCornerY;
	std::ofstream appConfig(s_app->m_appConfigPath, std::ios::trunc);
	appConfig << json.dump();
}

Vacuum::CApp* Vacuum::CApp::GetAppHandle()
{
	return s_app;
}

Vacuum::SWindowDimParams Vacuum::CApp::GetInitWindowDimParams() const
{
	return m_mainWindowDim;
}

Vacuum::CApp::CApp()
	:m_mainWindowDim(SWindowDimParams())
{
}
