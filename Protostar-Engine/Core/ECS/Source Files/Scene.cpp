#include "..\Header Files\Scene.h"
#include <fstream>
#include "Json.h"

const char* JSONSCENENAME = "name";
const char* JSONSCENEGUID = "guid";

Protostar::CScene::CScene(const std::filesystem::path& _scenePath)
	:CBaseObject("")
{
	m_objectPath = _scenePath;
	LoadData();
}

void Protostar::CScene::LoadData()
{
	std::ifstream objectFile(m_objectPath);

	Json json = {};
	objectFile >> json;

	m_objectName = json[JSONSCENENAME].get<std::string>();
	m_guid = json[JSONSCENEGUID].get<std::string>();
}

void Protostar::CScene::OnSave()
{
	std::ofstream objectFile(m_objectPath, std::ios::trunc);

	Json json = 
	{
		{JSONSCENENAME , m_objectName},
		{JSONSCENEGUID, m_guid.ToString()}
	};

	objectFile << json.dump();
}

void Protostar::CScene::Delete()
{
	std::filesystem::remove(m_objectPath);
}
