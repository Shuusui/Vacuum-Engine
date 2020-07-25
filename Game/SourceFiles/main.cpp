#include <Windows.h>
#include <iostream>
#include "Console.h"
#include "Guid.h"
#include "Log.h"
#include "Util.h"
#include "GlobalMacros.h"
#include "ThreadPool.h"

int32 WinMain(_In_ HINSTANCE _hInstance,_In_opt_  HINSTANCE _hPrevInstance,_In_ LPSTR _lpCmdLine,_In_ int _nShowCmd)
{
	using namespace Vacuum::Core;

	SGuid Guid = SGuid::NewGuid();

	SConsoleHandles handles = {};
	AllocateConsole(handles);

	std::string errorMsg;
	if (!CLog::Init(errorMsg))
	{
		std::cout << errorMsg;
		return -1;
	}

	CLog::RegisterHandle(SGuid::NewGuid(), handles.m_outputConInfo);

	std::wstring printfLog2 = PRINTF("Something to log here with number: %i and an additional name: %s", 13, "Blurensohn");

	CLog::Log(TEXT("First console log"));
	for (int i = 0; i < 100; ++i)
	{
		CLog::Log(printfLog2.c_str());
	}

	VE_LOG("this is pretty cool %i", 42);

	CThreadPool* threadPool = new CThreadPool(std::thread::hardware_concurrency());

	std::mutex* mutex = new std::mutex();
	auto lambda = [mutex]()->void
	{
		mutex->lock();
		VE_LOG("Just logging stuff");
		mutex->unlock();
	};
	std::packaged_task<void(void)> package(lambda);
	threadPool->QueueJob(new CJob<void>(package));


	return 0;
}