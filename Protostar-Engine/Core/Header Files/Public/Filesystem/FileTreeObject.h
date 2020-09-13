#pragma once
#include <filesystem>

namespace Protostar
{
	template<typename T>
	struct STreeNode
	{
		//STreeNode()
		//	:Path(std::filesystem::path())
		//	,Asset({})
		//{
		//}

		//STreeNode(const STreeNode& _other)
		//	:Path(_other.Path)
		//	,Asset(_other.Asset)
		//{
		//}

		std::filesystem::path Path;
		T Asset;
	};

	template<typename T>
	struct STreeObject
	{
		//STreeObject()
		//	: Path(std::filesystem::path())
		//	, SubDirs(std::vector<STreeObject<T>>())
		//	, Nodes(std::vector<STreeNode<T>>())
		//{
		//}

		//STreeObject(const STreeObject<T>& _other)
		//	: Path(_other.Path)
		//	, SubDirs(_other.SubDirs)
		//	, Nodes(_other.Nodes)
		//{
		//}

		std::filesystem::path Path;
		std::vector<STreeObject<T>> SubDirs;
		std::vector<STreeNode<T>> Nodes;
	};
}