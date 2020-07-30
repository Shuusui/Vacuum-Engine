#include <Windows.h>
#include <iostream>
#include "Console.h"
#include "Guid.h"
#include "Log.h"
#include "Util.h"
#include "GlobalMacros.h"
#include "GlobalDefs.h"
#include "ThreadPool.h"
#include "Window.h"



int32 WinMain(_In_ HINSTANCE _hInstance, _In_opt_  HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ int32 _nShowCmd)
{
	using namespace Vacuum::Core;

	SConsoleHandles handles = {};
	if (!AllocateConsole(handles))
	{
		CLog::LogDebugString(TEXT("Failed to Allocate console handles"));
		return -1;
	}

	std::wstring errorMsg = {};
	if (!CLog::Init(errorMsg))
	{
		CLog::LogDebugString(errorMsg);
		return -1;
	}

	SGuid handlesGuid = SGuid::NewGuid();
	CLog::RegisterHandle(handlesGuid, handles.m_outputConInfo);
	CThreadPool* threadPool = new CThreadPool(std::thread::hardware_concurrency());

	CWindow* newWindow = new CWindow(std::move(SWindowInfo()));
	if (!newWindow->Create(errorMsg))
	{
		VE_LOG(errorMsg);
		return -1;
	}
	newWindow->ShwoAndUpdate(_nShowCmd);
	while (true)
	{
		MSG msg = {};
		newWindow->RunWindow(msg);
	}

	delete threadPool;

	return 0;
}