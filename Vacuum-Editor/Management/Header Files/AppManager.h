#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "SharedStructs.h"
#include "GlobalDefs.h"
#include "Guid.h"
#include "Project.h"


namespace Vacuum
{
	struct SAppPaths
	{
		SAppPaths()
			:RootDir(std::filesystem::path())
			,ConfigDir(std::filesystem::path())
			,ProjectsDir(std::filesystem::path())
		{

		}
		std::filesystem::path RootDir;
		std::filesystem::path ConfigDir;
		std::filesystem::path ProjectsDir;
	};

	class CAppManager
	{
	public:
		static void InitApp();
		static void Destroy();
		static CAppManager* GetAppHandle();

		static SWindowDimParams GetInitWindowDimParams();
		static SAppPaths GetAppPaths();

		void LoadProject(CProject* _project);

		std::vector<CProject*> GetProjects() const 
		{
			return m_projects;
		}

		CProject* GetCurrentProject() const 
		{
			return m_currentProject;
		}
	private:
		CAppManager();
		void LoadRecentProject(const SGuid& _projectGuid);
		void LoadProjects();
		
		static CAppManager* s_app;
		SWindowDimParams m_mainWindowDim;
		SAppPaths m_appPaths;
		std::filesystem::path m_appConfigPath;
		CProject* m_currentProject;
		std::vector<CProject*> m_projects;
	};
}