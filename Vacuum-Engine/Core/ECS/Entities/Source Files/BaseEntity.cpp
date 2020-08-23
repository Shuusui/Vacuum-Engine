#include "..\Header Files\BaseEntity.h"
#include "ECS\Header Files\ECSManager.h"
#include "ECS\Header Files\EntityManager.h"
#include "ECS\Components\Header Files\Components.h"
#include "Json.h"
#include <fstream>

const char* JSONENTITYNAME = "name";
const char* JSONENTITYGUID = "guid";
const char* JSONTRANSFORM = "transform";

Vacuum::CBaseEntity::CBaseEntity(const std::string& _name)
	:CBaseObject(_name)
	,m_entity(CECSManager::CreateEntity())
{
	CEntityManager::GetHandle()->RegisterEntity(this);

}

Vacuum::CBaseEntity::CBaseEntity(const std::filesystem::path& _path)
	:CBaseObject(std::string())
	,m_entity(CECSManager::CreateEntity())
{
	m_objectPath = _path;
	LoadData();
	CEntityManager::GetHandle()->RegisterEntity(this);
}

Vacuum::CBaseEntity::~CBaseEntity()
{
}

void Vacuum::CBaseEntity::OnSave()
{
	std::ofstream objectFile(m_objectPath, std::ios::trunc);

	entt::registry& registry = CECSManager::GetRegistry();

	Json json =
	{
		{JSONENTITYNAME, m_objectName},
		{JSONENTITYGUID, m_guid.ToString()}
	};

	if (registry.has<CTransformComponent>(m_entity))
	{
		CTransformComponent transform = registry.get<CTransformComponent>(m_entity);
		transform.OnSave();
		json[JSONTRANSFORM] = transform.GetJsonObject();
	}


	objectFile << json.dump();
}

void Vacuum::CBaseEntity::LoadData()
{
	std::ifstream objectFile(m_objectPath);

	Json json = {};
	objectFile >> json;

	m_objectName = json[JSONENTITYNAME].get<std::string>();
	m_guid = json[JSONENTITYGUID].get<std::wstring>();
	entt::registry& registry = CECSManager::GetRegistry();
	if (json.contains(JSONTRANSFORM))
	{
		Json jsonTransform = json[JSONTRANSFORM].get<Json>();
		CTransformComponent transform = CTransformComponent(jsonTransform);
		CTransformComponent& blankTransform = registry.emplace<CTransformComponent>(m_entity);
		blankTransform = transform;
	}

}
