#include <Windows.h>
#include <iostream>
#include "Console.h"
#include "Guid.h"
#include "Log.h"

int WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nShowCmd) //TODO: warning
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