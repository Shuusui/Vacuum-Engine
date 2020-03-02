#include <Windows.h>
#include <iostream>
#include <Console.h>
#include "Guid.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) //TODO: warning
{
	using namespace Vacuum::Core;

	std::string errorStr;
	SGuid Guid = SGuid::NewGuid(errorStr);


	SConsoleHandles handles = {};
	CConsole::AllocateConsole(handles);

	std::wstring errorMsg;

	return 0;
}