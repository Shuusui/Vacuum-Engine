#include <Windows.h>
#include "Window.h"
#include <iostream>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	using namespace Vacuum::Core;
	std::string errorMsg;
	HWND wndHandle = CWindow::ConstructWindow(SWindowClassExInfo(), SWindowExInfo(),errorMsg);
	if (!wndHandle)
	{
		std::cout << errorMsg;
	}

	std::cin.get();
	return 0;
}