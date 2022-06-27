#pragma once

#include "..\Private\Entt\entt.hpp"

namespace Protostar
{
	class CECSManager
	{
	public:
		static void CreateECS();
		static entt::entity CreateEntity();
		static entt::registry& GetRegistry()
		{
			return s_ecsManager->m_registry;
		}
	private:
		static CECSManager* s_ecsManager;

		CECSManager();

		entt::registry m_registry; 
	};
}
