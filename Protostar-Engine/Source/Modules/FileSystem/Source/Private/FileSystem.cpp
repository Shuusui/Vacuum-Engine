#include "Filesystem.h"

inline bool Protostar::Core::Filesystem::HasParentDir(const std::filesystem::path& _path, const std::filesystem::path& _parentDir)
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

inline bool Protostar::Core::Filesystem::CheckExtensions(const std::filesystem::path& _path, const std::vector<std::filesystem::path>& _exts)
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
