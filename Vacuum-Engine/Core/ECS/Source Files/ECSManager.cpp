#include "..\Header Files\ECSManager.h"
#include "..\Header Files\EntityManager.h"

Vacuum::CECSManager* Vacuum::CECSManager::s_ecsManager = nullptr;

void Vacuum::CECSManager::CreateECS()
{
	if (s_ecsManager)
	{
		return;
	}
	s_ecsManager = new CECSManager();
	CEntityManager::CreateEntityManager();
}

entt::entity Vacuum::CECSManager::CreateEntity()
{
	return s_ecsManager->m_registry.create();
}

Vacuum::CECSManager::CECSManager()
{
}