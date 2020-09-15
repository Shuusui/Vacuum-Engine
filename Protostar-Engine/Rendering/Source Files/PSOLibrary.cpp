#include "..\Header Files\Public\PSOLibrary.h"

Protostar::CPSOLibrary* Protostar::CPSOLibrary::s_psoLibrary = nullptr;


void Protostar::CPSOLibrary::Create()
{
	if (s_psoLibrary)
	{
		return;
	}
	s_psoLibrary = new CPSOLibrary();
}

void Protostar::CPSOLibrary::Destroy()
{
	if (s_psoLibrary)
	{
		delete s_psoLibrary;
		s_psoLibrary = nullptr;
	}
}

Protostar::CPSOLibrary::~CPSOLibrary()
{

}

Protostar::SGuid Protostar::CPSOLibrary::CreatePSO()
{
	return SGuid();
}

Protostar::CPSOLibrary::CPSOLibrary()
{

}