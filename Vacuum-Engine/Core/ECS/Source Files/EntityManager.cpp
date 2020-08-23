#include "..\Header Files\EntityManager.h"
#include "ECS\Entities\Header Files\BaseEntity.h"

Vacuum::CEntityManager* Vacuum::CEntityManager::s_entityManager = nullptr;

void Vacuum::CEntityManager::CreateEntityManager()
{
	if (s_entityManager)
	{
		return;
	}
	s_entityManager = new CEntityManager();
}

void Vacuum::CEntityManager::RegisterEntity(CBaseEntity* _entity)
{
	m_entities.push_back(_entity);
}

std::vector<Vacuum::CBaseEntity*> Vacuum::CEntityManager::GetEntities() const
{
	return m_entities;
}

Vacuum::CEntityManager::~CEntityManager()
{

}

Vacuum::CEntityManager::CEntityManager()
{
}
