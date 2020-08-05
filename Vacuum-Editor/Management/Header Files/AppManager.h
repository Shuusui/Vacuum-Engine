#include <string>
#include <filesystem>
#include <unordered_map>
#include "SharedStructs.h"
#include "GlobalDefs.h"
#include "Guid.h"
#include "Project.h"


namespace Vacuum
{
	struct SAppPaths
	{
		SAppPaths()
			:m_rootDir(std::filesystem::path())
			,m_configDir(std::filesystem::path())
			,m_projectsDir(std::filesystem::path())
		{

		}
		std::filesystem::path m_rootDir;
		std::filesystem::path m_configDir;
		std::filesystem::path m_projectsDir;
	};

	class CAppManager
	{
	public:
		static void InitApp();
		static void Destroy();
		static CAppManager* GetAppHandle();

		static SWindowDimParams GetInitWindowDimParams();
		static SAppPaths GetAppPaths();

		void LoadProject();
		CProject* GetCurrentProject() const 
		{
			return m_currentProject;
		}
	private:
		CAppManager();
		
		static CAppManager* s_app;
		SWindowDimParams m_mainWindowDim;
		SAppPaths m_appPaths;
		std::filesystem::path m_appConfigPath;
		CProject* m_currentProject;
	};
}