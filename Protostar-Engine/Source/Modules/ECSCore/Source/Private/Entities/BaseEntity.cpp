#include "..\Header Files\BaseEntity.h"
#include "ECS\Header Files\ECSManager.h"
#include "ECS\Header Files\EntityManager.h"
#include "ECS\Components\Header Files\TransformComponent.h"
#include "ECS\Components\Header Files\MeshComponent.h"
#include "Json.h"
#include <fstream>
#include <filesystem>

Protostar::PBaseEntity::PBaseEntity(const std::string& _name)
	:PBaseObject(_name)
	,m_entity(CECSManager::CreateEntity())
{
	CEntityManager::GetHandle()->RegisterEntity(this);

}

Protostar::PBaseEntity::PBaseEntity(const std::filesystem::path& _path)
	:PBaseObject(std::string())
	,m_entity(CECSManager::CreateEntity())
{
	m_objectPath = _path;
	LoadData();
	CEntityManager::GetHandle()->RegisterEntity(this);
}

Protostar::PBaseEntity::~PBaseEntity()
{
}

void Protostar::PBaseEntity::OnSave()
{
}

void Protostar::PBaseEntity::LoadData()
{
}
