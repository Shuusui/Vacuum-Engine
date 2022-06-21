#include "..\Header Files\SavingSystem.h"

Protostar::PSavingSystem* Protostar::PSavingSystem::s_savingSystem = nullptr;

void Protostar::PSavingSystem::OnCreate()
{
	if (s_savingSystem)
	{
		return;
	}
	s_savingSystem = new PSavingSystem();
}

void Protostar::PSavingSystem::OnDestroy()
{
	if (s_savingSystem)
	{
		delete s_savingSystem;
		s_savingSystem = nullptr;
	}
}
