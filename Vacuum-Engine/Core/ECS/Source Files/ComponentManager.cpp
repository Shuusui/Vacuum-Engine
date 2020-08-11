#include "..\Header Files\ComponentManager.h"

Vacuum::CComponentManager* Vacuum::CComponentManager::s_componentManager = nullptr;

void Vacuum::CComponentManager::Create()
{
	if (s_componentManager)
	{
		return;
	}
	s_componentManager = new CComponentManager();
}

void Vacuum::CComponentManager::RegisterComponent(CBaseComponent* _compToRegister)
{
	const std::type_info& typeInfo = typeid(_compToRegister);
	auto comps = m_components.find(typeInfo.hash_code());
	if (comps == m_components.end())
	{
		m_components.insert(std::make_pair<size_t, std::vector<CBaseComponent*>>(typeInfo.hash_code(), std::vector<CBaseComponent*>{_compToRegister}));
		return;
	}
	comps->second.push_back(_compToRegister);
}

void Vacuum::CComponentManager::DeregisterComponent(CBaseComponent* _compToDeregister)
{
	const std::type_info& typeInfo = typeid(_compToDeregister);
	auto comps = m_components.find(typeInfo.hash_code());
	if (comps == m_components.end())
	{
		return;
	}
	comps->second.erase(std::remove_if(comps->second.begin(), comps->second.end(), [_compToDeregister](CBaseComponent* a)->bool{ return a == _compToDeregister; }));
}

Vacuum::CComponentManager::CComponentManager()
{

}
