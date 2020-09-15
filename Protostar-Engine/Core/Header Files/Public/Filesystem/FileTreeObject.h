#pragma once
#include <filesystem>

namespace Protostar
{
	template<typename T>
	struct STreeNode
	{
		std::filesystem::path Path;
		T Asset;
	};

	template<typename T>
	struct STreeObject
	{
		std::filesystem::path Path;
		std::vector<STreeObject<T>> SubDirs;
		std::vector<STreeNode<T>> Nodes;
	};
}