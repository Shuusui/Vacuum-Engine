#include "EntityManager.h"
#include "Entities\BaseEntity.h"

Protostar::CEntityManager* Protostar::CEntityManager::s_entityManager = nullptr;

void Protostar::CEntityManager::CreateEntityManager()
{
	if (s_entityManager)
	{
		return;
	}
	s_entityManager = new CEntityManager();
}

void Protostar::CEntityManager::RegisterEntity(PBaseEntity* _entity)
{
	m_entities.push_back(_entity);
}

std::vector<Protostar::PBaseEntity*> Protostar::CEntityManager::GetEntities() const
{
	return m_entities;
}

Protostar::CEntityManager::~CEntityManager()
{

}

Protostar::CEntityManager::CEntityManager()
{
}
