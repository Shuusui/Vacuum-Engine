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
#include "imgui.h"
#include "GUI.h"
#include "Timer.h"

int32 WinMain(_In_ HINSTANCE _hInstance, _In_opt_  HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ int32 _nShowCmd)
{
	using namespace Vacuum;

	CTimer::Create();

	SConsoleHandles handles = {};
	if (!AllocateConsole(handles))
	{
#if defined(_DEBUG)
		VE_DEBUG_LOG(TEXT("Failed to Allocate console handles"));
#endif
		return -1;
	}
	std::wstring errorMsg = {};
	if (!CLog::Init(errorMsg))
	{
#if defined(_DEBUG)
		VE_DEBUG_LOG(errorMsg);
#endif
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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	CGUI::Init(CMainWindow::GetWindowHandle()->GetHwnd());

	CRendererManager::Create(SRendererCreationInfo{ERenderAPIs::DX12, (uint32)appMgrHandle->GetInitWindowDimParams().m_width, (uint32)appMgrHandle->GetInitWindowDimParams().m_height, CMainWindow::GetWindowHandle()->GetHwnd()});
	CRendererManager::OnInit(appMgrHandle->GetCurrentProject()->GetShaderPaths());

	ImGui::StyleColorsDark();


	CMainWindow::ShowAndUpdate(_nShowCmd);
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		CTimer::Update();
		if (CMainWindow::RunWindow(msg))
		{
			continue;
		}
		CGUI::NewFrame();
	}

	CRendererManager::Destroy();
	CAppManager::Destroy();

	delete threadPool;

	return 0;
}