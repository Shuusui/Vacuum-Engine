#include "..\Header Files\Scene.h"
#include <fstream>
#include "Log.h"

Protostar::PScene::PScene(const std::filesystem::path& _scenePath)
	:PBaseObject("")
	,m_cameraEntity(nullptr)
{
	m_objectPath = _scenePath;
	LoadData();
}

void Protostar::PScene::LoadData()
{
}

void Protostar::PScene::OnSave()
{
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
