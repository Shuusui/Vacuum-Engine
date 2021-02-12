#include "..\Header Files\Scene.h"
#include <fstream>
#include "Json.h"
#include "Log.h"


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
	using namespace JsonKeys;
	m_objectName = json[JSONSCENENAME].get<std::string>();
	m_guid = json[JSONSCENEGUID].get<std::string>();
}

void Protostar::CScene::OnSave()
{
	std::ofstream objectFile(m_objectPath, std::ios::trunc);

	using namespace JsonKeys;
	Json json = 
	{
		{JSONSCENENAME , m_objectName},
		{JSONSCENEGUID, m_guid.ToString()}
	};

	objectFile << json.dump(0);
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
