#include "..\HeaderFiles\Public\App.h"

#pragma region Internal Includes 
#include "..\HeaderFiles\Public\Json.h"
#include "..\HeaderFiles\Public\Window.h"
#pragma endregion //Internal Includes

#pragma region External Includes
#include <filesystem>
#include <fstream>
#pragma endregion //External Includes


Vacuum::CApp* Vacuum::CApp::s_app = nullptr;

const std::wstring CONFIG_DIR = L"\\Configs\\";
const std::wstring APP_CONFIG = L"\\Configs\\app.config";

void Vacuum::CApp::InitApp()
{
	if (!s_app)
	{
		s_app = new CApp();
	}
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::filesystem::path configDir = currentPath.append(CONFIG_DIR);
	if (!std::filesystem::exists(configDir))
	{
		std::filesystem::create_directory(configDir);
	}
	std::filesystem::path appConfigPath = currentPath.append(APP_CONFIG);
	if (std::filesystem::exists(appConfigPath))
	{
		Json json = {};
		std::ifstream appConfig(appConfigPath);
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

	std::filesystem::path currentPath = std::filesystem::current_path();
	std::filesystem::path configDir = currentPath.append(CONFIG_DIR);
	if (!std::filesystem::exists(configDir))
	{
		std::filesystem::create_directory(configDir);
	}
	std::filesystem::path appConfigPath = currentPath.append(APP_CONFIG);
	Json json = {};
	json["height"] = s_app->m_mainWindowDim.m_height;
	json["width"] = s_app->m_mainWindowDim.m_width;
	json["x"] = s_app->m_mainWindowDim.m_leftTopCornerX;
	json["y"] = s_app->m_mainWindowDim.m_leftTopCornerY;
	std::ofstream appConfig(appConfigPath, std::ios::trunc);
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
