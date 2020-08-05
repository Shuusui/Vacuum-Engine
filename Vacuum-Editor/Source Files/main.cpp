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
#include "AppManager.h"
#include "SharedStructs.h"
#include "RendererManager.h"

int32 WinMain(_In_ HINSTANCE _hInstance, _In_opt_  HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ int32 _nShowCmd)
{
	using namespace Vacuum;
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

	CLog::RegisterHandle(handles.m_handlesGuid, handles.m_outputConInfo);

	CAppManager::InitApp();

	CAppManager* appMgrHandle = CAppManager::GetAppHandle();

	CThreadPool* threadPool = new CThreadPool(std::thread::hardware_concurrency());

	SWindowInfo windowInfo = {};
	windowInfo.m_classParams.m_className = TEXT("Vacuum Engine window");
	windowInfo.m_classParams.m_hInstance = _hInstance;
	windowInfo.m_classParams.m_backgroundColor = CreateSolidBrush(RGB(1,1,1));
	windowInfo.m_classParams.m_style = CS_HREDRAW | CS_VREDRAW;
	windowInfo.m_creationParams.m_dwExStyle = NULL;
	windowInfo.m_creationParams.m_dwStyle = WS_OVERLAPPEDWINDOW;
	windowInfo.m_creationParams.m_windowName = TEXT("Vacuum-Engine");
	windowInfo.m_creationParams.m_parentWindow = nullptr;
	windowInfo.m_creationParams.m_menu = nullptr;
	windowInfo.m_creationParams.m_lpParam = nullptr;
	windowInfo.m_dimParams = appMgrHandle->GetInitWindowDimParams();

	CMainWindow::InitWindow(windowInfo);
	if (!CMainWindow::Create(errorMsg))
	{
		VE_LOG(errorMsg);
		return -1;
	}

	CMainWindow::ShowAndUpdate(_nShowCmd);

	CRendererManager::Create(SRendererCreationInfo{ERenderAPIs::DX12, (uint32)appMgrHandle->GetInitWindowDimParams().m_width, (uint32)appMgrHandle->GetInitWindowDimParams().m_height, CMainWindow::GetWindowHandle()->GetHwnd()});
	CRendererManager::OnInit(appMgrHandle->GetCurrentProject()->GetShaderPaths());
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		CMainWindow::RunWindow(msg);
		CRendererManager::OnRender();
	}

	CRendererManager::Destroy();
	CAppManager::Destroy();

	delete threadPool;

	return 0;
}