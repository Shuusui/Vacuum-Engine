#include "..\Header Files\Scene.h"
#include <fstream>
#include "Json.h"
#include "Log.h"

const char* JSONSCENENAME = "name";
const char* JSONSCENEGUID = "guid";

Protostar::PScene::PScene(const std::filesystem::path& _scenePath)
	:PBaseObject("")
	,m_cameraEntity(nullptr)
{
	m_objectPath = _scenePath;
	LoadData();
}

void Protostar::PScene::LoadData()
{
	std::ifstream objectFile(m_objectPath);

	PJson json = {};
	objectFile >> json;

	m_objectName = json[JSONSCENENAME].get<std::string>();
	m_guid = json[JSONSCENEGUID].get<std::string>();
}

void Protostar::PScene::OnSave()
{
	std::ofstream objectFile(m_objectPath, std::ios::trunc);

	PJson json =
	{
		{JSONSCENENAME , m_objectName},
		{JSONSCENEGUID, m_guid.ToString()}
	};

	objectFile << json.dump();
}

void Protostar::PScene::RenderActiveCamera()
{
	if (!m_cameraEntity)
	{
		PE_LOG("No active camera set");
		return;
	}
}

void Protostar::PScene::SetActiveCamera(PBaseEntity* _cameraEntity)
{
	m_cameraEntity = _cameraEntity;
}

void Protostar::PScene::Delete()
{
	std::filesystem::remove(m_objectPath);
}
