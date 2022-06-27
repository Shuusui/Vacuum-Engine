#include "ECSManager.h"
#include "EntityManager.h"

Protostar::CECSManager* Protostar::CECSManager::s_ecsManager = nullptr;

void Protostar::CECSManager::CreateECS()
{
	if (s_ecsManager)
	{
		return;
	}
	s_ecsManager = new CECSManager();
	CEntityManager::CreateEntityManager();
}

entt::entity Protostar::CECSManager::CreateEntity()
{
	return s_ecsManager->m_registry.create();
}

Protostar::CECSManager::CECSManager()
{
}