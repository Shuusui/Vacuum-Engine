#pragma once

#ifdef FILESYSTEM_LIBRARY
	#define FILESYSTEM_API __declspec(dllexport)
#else
	#define FILESYSTEM_API __declspec(dllimport)
#endif

#include <filesystem>
#include <functional>
#include <fstream>
#include "FileTreeObject.h"
#include "Logger.h"

namespace Protostar::Core
{
	class FILESYSTEM_API Filesystem
	{
	public:
		template<typename T>
		static TreeObject<T> GenerateFileTree(const std::filesystem::path& _basePath, const std::function<bool(const std::filesystem::path&, TreeNode<T>&)>& _predicate = {}, const std::vector<std::filesystem::path>& _exts = {})
		{
			TreeObject<T> treeObject = TreeObject<T>();
			treeObject.Path = _basePath;

			if (!_predicate)
			{
				PE_LOG("Unable to create tree object since predicate is nullptr");
				return treeObject;
			}

			for (const std::filesystem::path& path : std::filesystem::directory_iterator(_basePath))
			{
				if (std::filesystem::is_directory(path))
				{
					TreeObject<T> subTree = TreeObject<T>();
					subTree.Path = path;
					FillTreeObject(subTree, path, _predicate, _exts);
					treeObject.SubDirs.push_back(subTree);
					continue;
				}

				if (_exts.size() > 0)
				{
					if (!CheckExtensions(path, _exts))
					{
						continue;
					}
				}

				if (_predicate)
				{
					TreeNode<T> node = {};
					node.Path = path;
					if (_predicate(path, node))
					{
						treeObject.Nodes.push_back(node);
					}
				}
			}

			return treeObject;
		}

		static bool HasParentDir(const std::filesystem::path& _path, const std::filesystem::path& _parentDir);

	private:
		template<typename T>
		static void FillTreeObject(TreeObject<T>& _treeObj, const std::filesystem::path& _path, const std::function<bool(const std::filesystem::path&, TreeNode<T>&)>& _predicate, const std::vector<std::filesystem::path>& _exts)
		{
			for (const std::filesystem::path& path : std::filesystem::directory_iterator(_path))
			{
				if(std::filesystem::is_directory(path))
				{
					TreeObject<T> subTree = TreeObject<T>();
					subTree.Path = path;
					FillTreeObject(subTree, path, _predicate, _exts);
					_treeObj.SubDirs.push_back(subTree);
					continue;
				}

				if (_exts.size() > 0)
				{
					if (!CheckExtensions(path, _exts))
					{
						continue;
					}
				}

				TreeNode<T> node = {};
				node.Path = path;
				if (_predicate(path, node))
				{
					_treeObj.Nodes.push_back(node);
				}
			}
		}

		static bool CheckExtensions(const std::filesystem::path& _path, const std::vector<std::filesystem::path>& _exts);
	};
}
