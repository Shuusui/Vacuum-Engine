#pragma once
#include <unordered_map>
#include <filesystem>
#include "Guid.h"
#include <d3d12.h>
#include "SharedStructs.h"

namespace Protostar
{
	class CPSOLibrary
	{
	public:
		static void Create(const std::filesystem::path& _projectPath);
		static CPSOLibrary* GetHandle()
		{
			return s_psoLibrary;
		}
		static void Destroy();

		~CPSOLibrary();
		SGuid CreatePSO(const SPSOInfo& _psoInfo);
	private:
		void Load();
		void Save();
		void CreateDefaultPSO();
		static CPSOLibrary* s_psoLibrary;
		CPSOLibrary(const std::filesystem::path& _projectPath);
		std::unordered_map<SGuid, SPSOInfo> m_PSOs;
		std::filesystem::path m_projectPath;
		std::filesystem::path m_projectConfigPath;
		std::filesystem::path m_psoLibraryIniPath;
	};
}