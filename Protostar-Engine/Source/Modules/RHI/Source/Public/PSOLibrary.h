#pragma once

#include <unordered_map>
#include <filesystem>
#include "Guid.h"
#include "SharedStructs.h"

namespace Protostar::Core
{
	//class PSOLibrary
	//{
	//public:
	//	static void Create(const std::filesystem::path& _projectPath);
	//	static PSOLibrary* GetHandle()
	//	{
	//		return s_psoLibrary;
	//	}
	//	static void Destroy();

	//	~PSOLibrary();
	//	Guid CreatePSO(const PPSOInfo& _psoInfo);
	//private:
	//	void Load();
	//	void Save();
	//	void CreateDefaultPSO();
	//	static PSOLibrary* s_psoLibrary;
	//	PSOLibrary(const std::filesystem::path& _projectPath);
	//	std::unordered_map<Guid, PPSOInfo> m_PSOs;
	//	std::filesystem::path m_projectPath;
	//	std::filesystem::path m_projectConfigPath;
	//	std::filesystem::path m_psoLibraryIniPath;
	//};
}
