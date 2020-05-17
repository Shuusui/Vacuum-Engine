#include <Windows.h>
#include <iostream>
#include "Console.h"
#include "Guid.h"
#include "Log.h"

int32 WinMain(_In_ HINSTANCE _hInstance,_In_opt_  HINSTANCE _hPrevInstance,_In_ LPSTR _lpCmdLine,_In_ int _nShowCmd) //TODO: warning
{
	using namespace Vacuum::Core;

	SGuid Guid = SGuid::NewGuid();


	SConsoleHandles handles = {};
	CConsole::AllocateConsole(handles);

	std::string errorMsg;
	if (!CLog::Init(errorMsg))
	{
		std::cout << errorMsg; 
	}

	CLog::RegisterHandle(SGuid::NewGuid(), handles.m_outputHandle);

	CLog::Log(TEXT("my first console log to my own console"));


	return 0;
}