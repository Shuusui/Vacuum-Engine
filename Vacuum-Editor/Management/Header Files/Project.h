#pragma once
#include <filesystem>
#include "Guid.h"

namespace Vacuum
{
	class CProject
	{
		CProject(const std::filesystem::path& _projectPath);
		CProject(const std::filesystem::path& _projectPath, const SGuid& _projectGuid);
	private:
		std::wstring m_name;
		SGuid m_guid;
		std::filesystem::path m_currentProjectDir;
		std::filesystem::path m_currentContentDir;
		std::filesystem::path m_currentShaderDir;
	};
}