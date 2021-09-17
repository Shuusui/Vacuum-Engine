#include "..\Header Files\BaseEntity.h"
#include "ECS\Header Files\ECSManager.h"
#include "ECS\Header Files\EntityManager.h"
#include "ECS\Components\Header Files\TransformComponent.h"
#include "ECS\Components\Header Files\MeshComponent.h"
#include "Json.h"
#include <fstream>

const char* JSONENTITYNAME = "name";
const char* JSONENTITYGUID = "guid";
const char* JSONTRANSFORM = "transform";
const char* JSONMESH = "mesh";

Protostar::CBaseEntity::CBaseEntity(const std::string& _name)
	:CBaseObject(_name)
	,m_entity(CECSManager::CreateEntity())
{
	CEntityManager::GetHandle()->RegisterEntity(this);
}

Protostar::CBaseEntity::CBaseEntity(const std::filesystem::path& _path)
	:CBaseObject(std::string())
	,m_entity(CECSManager::CreateEntity())
{
	m_objectPath = _path;
	LoadData();
	CEntityManager::GetHandle()->RegisterEntity(this);
}

Protostar::CBaseEntity::~CBaseEntity()
{
}

void Protostar::CBaseEntity::OnSave()
{

	entt::registry& registry = CECSManager::GetRegistry();

	if (!m_oldPath.empty())
	{
		std::filesystem::remove(m_oldPath);
		m_oldPath.clear();
		m_oldName.clear();
		m_objectPath.replace_extension(".veentity");
	}
	std::ofstream objectFile(m_objectPath, std::ios::trunc);

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

	if (registry.has<CMeshComponent>(m_entity))
	{
		CMeshComponent mesh = registry.get<CMeshComponent>(m_entity);
		mesh.OnSave();
		json[JSONMESH] = mesh.GetJsonObject();
	}

	objectFile << json.dump();
}

void Protostar::CBaseEntity::LoadData()
{
	std::ifstream objectFile(m_objectPath);

	Json json = {};
	objectFile >> json;

	m_objectName = json[JSONENTITYNAME].get<std::string>();
	m_guid = json[JSONENTITYGUID].get<std::string>();
	entt::registry& registry = CECSManager::GetRegistry();
	if (json.contains(JSONTRANSFORM))
	{
		Json jsonTransform = json[JSONTRANSFORM].get<Json>();
		CTransformComponent transform = CTransformComponent(jsonTransform);
		CTransformComponent& blankTransform = registry.emplace<CTransformComponent>(m_entity);
		blankTransform = transform;
	}

	if (json.contains(JSONMESH))
	{
		Json jsonMesh = json[JSONMESH].get<Json>();
		CMeshComponent meshComp = CMeshComponent(jsonMesh);
		CMeshComponent& blankMeshComp = registry.emplace<CMeshComponent>(m_entity);
		blankMeshComp = meshComp;
	}
}
