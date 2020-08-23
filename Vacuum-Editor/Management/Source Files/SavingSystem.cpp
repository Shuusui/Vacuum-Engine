#include "..\Header Files\SavingSystem.h"

Vacuum::CSavingSystem* Vacuum::CSavingSystem::s_savingSystem = nullptr;

void Vacuum::CSavingSystem::OnCreate()
{
	if (s_savingSystem)
	{
		return;
	}
	s_savingSystem = new CSavingSystem();
}

void Vacuum::CSavingSystem::OnDestroy()
{
	if (s_savingSystem)
	{
		delete s_savingSystem;
		s_savingSystem = nullptr;
	}
}
