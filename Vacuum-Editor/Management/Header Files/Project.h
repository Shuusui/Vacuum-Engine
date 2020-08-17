#pragma once
#include <filesystem>
#include "Guid.h"

namespace Vacuum
{
	class CProject
	{
	public:
		CProject(const std::filesystem::path& _projectPath);
		std::vector<std::filesystem::path> GetShaderPaths() const
		{
			return m_shaderPaths;
		}

		std::vector<std::filesystem::path> GetModelPaths() const
		{
			return m_modelPaths;
		}

		SGuid GetGuid() const 
		{
			return m_guid;
		}

		std::string GetName() const
		{
			return m_name;
		}

	private:
		SGuid m_guid;
		std::string m_name;
		std::filesystem::path m_configsDir;
		std::filesystem::path m_currentProjectDir;
		std::filesystem::path m_currentContentDir;
		std::filesystem::path m_currentShaderDir;
		std::filesystem::path m_modelDir;
		std::filesystem::path m_projectFile;
		std::vector<std::filesystem::path> m_shaderPaths;
		std::vector<std::filesystem::path> m_modelPaths;
	};
}