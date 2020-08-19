#pragma once
#include <filesystem>
#include "Guid.h"

namespace Vacuum
{
	struct SProjectPaths
	{
		std::filesystem::path ConfigDir;
		std::filesystem::path ProjectDir;
		std::filesystem::path ContentDir;
		std::filesystem::path ShaderDir;
		std::filesystem::path ModelsDir;
		std::filesystem::path ProjectFilePath;
	};

	class CProject
	{
	public:
		CProject(const std::filesystem::path& _projectPath);
		~CProject();

		SGuid GetGuid() const 
		{
			return m_guid;
		}

		std::string GetName() const
		{
			return m_name;
		}

		SProjectPaths GetProjectPaths() const 
		{
			return m_projectPaths;
		}

	private:
		SGuid m_guid;
		SGuid m_mostRecentSceneGuid;
		std::string m_name;
		SProjectPaths m_projectPaths;
	};
}