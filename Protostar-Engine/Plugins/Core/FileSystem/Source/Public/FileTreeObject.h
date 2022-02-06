#ifndef FILETREEOBJECT_H
#define FILETREEOBJECT_H

#include <filesystem>
#include <vector>

namespace Protostar
{
	template<typename T>
	struct PTreeNode
	{
		std::filesystem::path Path;
		T Asset;
	};

	template<typename T>
	struct PTreeObject
	{
		std::filesystem::path Path;
		std::vector<PTreeObject<T>> SubDirs;
		std::vector<PTreeNode<T>> Nodes;
	};
}
#endif //FILETREEOBJECT_H