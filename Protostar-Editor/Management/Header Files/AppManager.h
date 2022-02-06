#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "SharedStructs.h"
#include "GlobalDefinitions.h"
#include "Guid.h"
#include "Project.h"
#include <functional>


namespace Protostar
{
	struct PAppPaths
	{
		PAppPaths()
			:RootDir(std::filesystem::path())
			,ConfigDir(std::filesystem::path())
			,ProjectsDir(std::filesystem::path())
		{

		}
		std::filesystem::path RootDir;
		std::filesystem::path ConfigDir;
		std::filesystem::path ProjectsDir;
	};

	class PAppManager
	{
	public:
		static bool InitApp(const HINSTANCE& _hInstance);
		static void Destroy();
		static PAppManager* GetAppHandle();

		static SWindowDimParams GetInitWindowDimParams();
		static PAppPaths GetAppPaths();

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

		void LoadProject(PProject* _project);

		std::vector<PProject*> GetProjects() const 
		{
			return m_projects;
		}

		PProject* GetCurrentProject() const 
		{
			return m_currentProject;
		}

		size_t RegisterOnLoadProjectCallback(const std::function<void(PProject*, PProject*)>& _onLoadCallback)
		{
			m_registeredOnLoadProjectCallbacks.push_back(_onLoadCallback);
			return m_registeredOnLoadProjectCallbacks.size() - 1;
		}
		void RemoveOnLoadProjectCallback(const size_t& _callbackIndex)
		{
			m_registeredOnLoadProjectCallbacks.erase(m_registeredOnLoadProjectCallbacks.begin() + _callbackIndex);
		}

		~PAppManager();
	private:
		PAppManager();
		void LoadRecentProject(const PGuid& _projectGuid);
		void LoadProjects();
		
		static PAppManager* s_app;
		SWindowDimParams m_mainWindowDim;
		PAppPaths m_appPaths;
		std::filesystem::path m_appConfigPath;
		PProject* m_currentProject;
		std::vector<PProject*> m_projects;
		std::vector<std::function<void(PProject*, PProject*)>> m_registeredOnLoadProjectCallbacks;
		bool m_bLastVSyncState;
	};
}