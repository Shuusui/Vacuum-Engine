#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include "Guid.h"
#include "..\Private\Project.h"

namespace Vacuum
{
	class CProjectManager
	{
	public:
		static void Create(const std::wstring& _rootDir);
		void OpenProject(const SGuid& _projectGuid);
		void CreateNewProject(const std::wstring& _projectName);
		static void Destroy();
	private:
		CProjectManager(const std::wstring& _rootDir);

		static CProjectManager* s_projectManager;
		std::filesystem::path m_projectsDir;
		CProject* m_currentProject;
	};
}