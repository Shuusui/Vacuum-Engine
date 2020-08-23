#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "SharedStructs.h"
#include "GlobalDefs.h"
#include "Guid.h"
#include "Project.h"
#include <functional>


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

		static void SetLastVSyncState(const bool& bVSyncState)
		{
			if (!s_app)
			{
				return;
			}
			s_app->m_bLastVSyncState = bVSyncState;
		}
		static bool GetLastVSyncState()
		{

			if (!s_app)
			{
				return false;
			}
			return s_app->m_bLastVSyncState;
		}

		void LoadProject(CProject* _project);

		std::vector<CProject*> GetProjects() const 
		{
			return m_projects;
		}

		CProject* GetCurrentProject() const 
		{
			return m_currentProject;
		}

		size_t RegisterOnLoadProjectCallback(const std::function<void(CProject*, CProject*)>& _onLoadCallback)
		{
			m_registeredOnLoadProjectCallbacks.push_back(_onLoadCallback);
			return m_registeredOnLoadProjectCallbacks.size() - 1;
		}
		void RemoveOnLoadProjectCallback(const size_t& _callbackIndex)
		{
			m_registeredOnLoadProjectCallbacks.erase(m_registeredOnLoadProjectCallbacks.begin() + _callbackIndex);
		}

		~CAppManager();
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
		std::vector<std::function<void(CProject*, CProject*)>> m_registeredOnLoadProjectCallbacks;
		bool m_bLastVSyncState;
	};
}