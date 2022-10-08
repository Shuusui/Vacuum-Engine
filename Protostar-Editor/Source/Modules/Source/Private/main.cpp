#include <Windows.h>
#include <iostream>
#include "GlobalDefinitions.h"
#include "App.h"

#pragma warning(disable : 4100)
s32 WinMain(_In_ HINSTANCE _hInstance, _In_opt_  HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ s32 _nShowCmd)
#pragma warning(default : 4100)
{
	using namespace Protostar;

	std::string errorMsg;
	Core::App::Init(_hInstance, _nShowCmd, errorMsg);
	const Core::App& app = Core::App::Get();
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		app.Update(msg);
	}
	return 0;
}