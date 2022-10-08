#pragma once

#include <filesystem>
#include <vector>

namespace Protostar::Core
{
	template<typename T>
	struct TreeNode
	{
		std::filesystem::path Path;
		T Asset;
	};

	template<typename T>
	struct TreeObject
	{
		std::filesystem::path Path;
		std::vector<TreeObject<T>> SubDirs;
		std::vector<TreeNode<T>> Nodes;
	};
}
