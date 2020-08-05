#include <string>
#include <filesystem>
#include <unordered_map>
#include "SharedStructs.h"
#include "GlobalDefs.h"
#include "Guid.h"


namespace Vacuum
{
	struct SAppPaths
	{
		std::filesystem::path m_rootDir;
		std::filesystem::path m_configDir;
		std::filesystem::path m_projectsDir;
		std::pair<SGuid, std::filesystem::path> m_currentProject;
		std::unordered_map<SGuid, std::filesystem::path> m_recentProjects;
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