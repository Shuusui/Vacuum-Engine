#pragma once
#include "FileTreeObject.h"
import <filesystem>;
import <functional>;
import <fstream>;

namespace Protostar
{
	class CFilesystem
	{
	public:
		template<typename T>
		static STreeObject<T> GenerateFileTree(const std::filesystem::path& _basePath, const std::function<bool(const std::filesystem::path&, STreeNode<T>&)>& _predicate = {}, const std::vector<std::filesystem::path>& _exts = {})
		{
			STreeObject<T> treeObject = STreeObject<T>();
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
					STreeObject<T> subTree = STreeObject<T>();
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
					STreeNode<T> node = {};
					node.Path = path;
					if (_predicate(path, node))
					{
						treeObject.Nodes.push_back(node);
					}
				}
			}

			return treeObject;
		}

		static bool HasParentDir(const std::filesystem::path& _path, const std::filesystem::path& _parentDir)
		{
			if (!_path.has_parent_path() || _path == std::filesystem::current_path() || _path == _path.root_path())
			{
				return false;
			}

			std::filesystem::path parentPath = _path.parent_path();
			if (parentPath.filename() != _parentDir)
			{
				return HasParentDir(parentPath, _parentDir);
			}
			return true;
		}

	private:
		template<typename T>
		static void FillTreeObject(STreeObject<T>& _treeObj, const std::filesystem::path& _path, const std::function<bool(const std::filesystem::path&, STreeNode<T>&)>& _predicate, const std::vector<std::filesystem::path>& _exts)
		{
			for (const std::filesystem::path& path : std::filesystem::directory_iterator(_path))
			{
				if(std::filesystem::is_directory(path))
				{
					STreeObject<T> subTree = STreeObject<T>();
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

				STreeNode<T> node = {};
				node.Path = path;
				if (_predicate(path, node))
				{
					_treeObj.Nodes.push_back(node);
				}
			}
		}

		static bool CheckExtensions(const std::filesystem::path& _path, const std::vector<std::filesystem::path>& _exts)
		{
			for (const std::filesystem::path& ext : _exts)
			{
				if (_path.extension() != ext)
				{
					return false;
				}
			}
			return true;
		}
	};
}