#include <Windows.h>
#include "Window.h"
#include <iostream>
#include <Console.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) //TODO: warning
{
	using namespace Vacuum::Core;

	SConsoleHandles handles = {};
	CConsole::AllocateConsole(handles);

	std::wstring errorMsg;
	HWND wndHandle = CWindow::ConstructWindow(SWindowClassExInfo(), SWindowExInfo(), errorMsg);
	if (!wndHandle)
	{
		std::wcout << errorMsg;
	}

	return 0;
}