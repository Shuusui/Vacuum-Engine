#include "..\Header Files\EntityManager.h"
#include "ECS\Entities\Header Files\BaseEntity.h"

Protostar::CEntityManager* Protostar::CEntityManager::s_entityManager = nullptr;

void Protostar::CEntityManager::CreateEntityManager()
{
	if (s_entityManager)
	{
		return;
	}
	s_entityManager = new CEntityManager();
}

void Protostar::CEntityManager::RegisterEntity(CBaseEntity* _entity)
{
	m_entities.push_back(_entity);
}

std::vector<Protostar::CBaseEntity*> Protostar::CEntityManager::GetEntities() const
{
	return m_entities;
}

Protostar::CEntityManager::~CEntityManager()
{

}

Protostar::CEntityManager::CEntityManager()
{
}
