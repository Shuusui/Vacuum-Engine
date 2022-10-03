#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "Project.h"
#include <functional>
#include "Window.h"

struct Guid;

namespace Protostar::Editor
{
	struct AppPaths
	{
		AppPaths()
			:RootDir(std::filesystem::path())
			,ConfigDir(std::filesystem::path())
			,ProjectsDir(std::filesystem::path())
		{

		}
		std::filesystem::path RootDir;
		std::filesystem::path ConfigDir;
		std::filesystem::path ProjectsDir;
	};

	class AppManager
	{
	};
}