#include "..\Header Files\SavingSystem.h"

Protostar::CSavingSystem* Protostar::CSavingSystem::s_savingSystem = nullptr;

void Protostar::CSavingSystem::OnCreate()
{
	if (s_savingSystem)
	{
		return;
	}
	s_savingSystem = new CSavingSystem();
}

void Protostar::CSavingSystem::OnDestroy()
{
	if (s_savingSystem)
	{
		delete s_savingSystem;
		s_savingSystem = nullptr;
	}
}
