#ifndef PSOLIBRARY_H
#define PSOLIBRARY_H

#include <unordered_map>
#include <filesystem>
#include "Guid.h"
#include "SharedStructs.h"

namespace Protostar
{
	class PPSOLibrary
	{
	public:
		static void Create(const std::filesystem::path& _projectPath);
		static PPSOLibrary* GetHandle()
		{
			return s_psoLibrary;
		}
		static void Destroy();

		~PPSOLibrary();
		PGuid CreatePSO(const PPSOInfo& _psoInfo);
	private:
		void Load();
		void Save();
		void CreateDefaultPSO();
		static PPSOLibrary* s_psoLibrary;
		PPSOLibrary(const std::filesystem::path& _projectPath);
		std::unordered_map<PGuid, PPSOInfo> m_PSOs;
		std::filesystem::path m_projectPath;
		std::filesystem::path m_projectConfigPath;
		std::filesystem::path m_psoLibraryIniPath;
	};
}
#endif //PSOLIBRARY_H