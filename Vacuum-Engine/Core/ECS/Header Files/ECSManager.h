#pragma once
#include "ECS/Entt/Header Files/entt.hpp"

namespace Vacuum
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