#include <Windows.h>
#include <iostream>
#include "Console.h"
#include "Guid.h"
#include "Log.h"
#include "Util.h"

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
		return -1;
	}

	CLog::RegisterHandle(SGuid::NewGuid(), handles.m_outputHandle);

	std::wstring printfLog = Vacuum::Core::Printf(TEXT("Something to log here with number: %i and an additional name: %s"), 42, "Dr.House");

	CLog::Log(TEXT("First console log"));
	CLog::Log(printfLog.c_str());

	return 0;
}