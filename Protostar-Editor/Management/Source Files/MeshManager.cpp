#include "..\Header Files\MeshManager.h"
#include <fstream>
#include <unordered_set>

Protostar::PMeshManager* Protostar::PMeshManager::s_meshManager = nullptr;

void Protostar::PMeshManager::OnCreate(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath)
{
	if (s_meshManager)
	{
		delete s_meshManager;
		s_meshManager = nullptr;
	}
	s_meshManager = new PMeshManager(_meshesPath, _configsPath);
}

void Protostar::PMeshManager::OnDestroy()
{
	if (s_meshManager)
	{
		delete s_meshManager;
		s_meshManager = nullptr;
	}
}

void Protostar::PMeshManager::Load()
{
	
}

void Protostar::PMeshManager::Save()
{

}

Protostar::PMeshManager::~PMeshManager()
{
	Save();
}

Protostar::PMeshManager::PMeshManager(const std::filesystem::path& _meshesPath, const std::filesystem::path& _configsPath)
	:m_meshesPath(_meshesPath)
	,m_configsPath(_configsPath)
	,m_configFilePath(m_configsPath / "meshmanager.ini")
{
	Load();
}
