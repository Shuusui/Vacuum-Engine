#include <Windows.h>
#include "Window.h"
#include <iostream>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) //TODO: warning
{
	using namespace Vacuum::Core;
	std::wstring errorMsg;
	HWND wndHandle = CWindow::ConstructWindow(SWindowClassExInfo(), SWindowExInfo(), errorMsg);
	if (!wndHandle)
	{
		std::wcout << errorMsg;
	}

	std::cin.get();
	return 0;
}