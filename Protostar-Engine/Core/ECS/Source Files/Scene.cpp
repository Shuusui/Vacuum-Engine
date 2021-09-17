#include "..\Header Files\Scene.h"
#include "Json.h"
#include "Log.h"
import <fstream>;

const char* JSONSCENENAME = "name";
const char* JSONSCENEGUID = "guid";

Protostar::CScene::CScene(const std::filesystem::path& _scenePath)
	:CBaseObject("")
	,m_cameraEntity(nullptr)
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

void Protostar::CScene::RenderActiveCamera()
{
	if (!m_cameraEntity)
	{
		PE_LOG("No active camera set");
		return;
	}
}

void Protostar::CScene::SetActiveCamera(CBaseEntity* _cameraEntity)
{
	m_cameraEntity = _cameraEntity;
}

void Protostar::CScene::Delete()
{
	std::filesystem::remove(m_objectPath);
}
