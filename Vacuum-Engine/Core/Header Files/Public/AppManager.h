#pragma once
#include <string>
#include "GlobalDefs.h"
#include "SharedStructs.h"
#include <filesystem>

namespace Vacuum
{
	struct SAppPaths
	{
		std::filesystem::path m_rootDir;
		std::filesystem::path m_configDir;
	};

	class CAppManager
	{
	public:
		static void InitApp();
		static void Destroy();
		static CAppManager* GetAppHandle();

		static SWindowDimParams GetInitWindowDimParams();
		static SAppPaths GetAppPaths();
	private:
		CAppManager();
		
		static CAppManager* s_app;
		SWindowDimParams m_mainWindowDim;
		SAppPaths m_appPaths;
		std::filesystem::path m_appConfigPath;
	};
}