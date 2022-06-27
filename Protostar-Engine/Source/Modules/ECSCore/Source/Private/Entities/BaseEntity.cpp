#include "Entities\BaseEntity.h"
#include "ECSManager.h"
#include "EntityManager.h"
#include "Components\TransformComponent.h"
#include "Components\MeshComponent.h"
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
