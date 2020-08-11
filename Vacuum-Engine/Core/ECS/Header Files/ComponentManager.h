#pragma once
#include "..\Components\Header Files\BaseComponent.h"
#include <unordered_map>

namespace Vacuum
{
	class CComponentManager
	{
		static void Create();
		void RegisterComponent(CBaseComponent* _compToRegister);
		void DeregisterComponent(CBaseComponent* _compToDeregister);

	private:
		CComponentManager();
		static CComponentManager* s_componentManager;

		std::unordered_map<size_t, std::vector<CBaseComponent*>> m_components;
	};
}