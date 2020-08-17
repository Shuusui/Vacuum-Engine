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

	CLog::RegisterHandle(handles.HandlesGuid, handles.OutputConInfo);

	CAppManager::InitApp();

	CAppManager* appMgrHandle = CAppManager::GetAppHandle();

	CThreadPool* threadPool = new CThreadPool(std::thread::hardware_concurrency());

	SWindowInfo windowInfo = {};
	windowInfo.ClassParams.ClassName = TEXT("Vacuum Engine window");
	windowInfo.ClassParams.HInstance = _hInstance;
	windowInfo.ClassParams.BackgroundColor = CreateSolidBrush(RGB(1,1,1));
	windowInfo.ClassParams.Style = CS_HREDRAW | CS_VREDRAW;
	windowInfo.CreationParams.DwExStyle = NULL;
	windowInfo.CreationParams.DwStyle = WS_OVERLAPPEDWINDOW;
	windowInfo.CreationParams.WindowName = TEXT("Vacuum-Engine");
	windowInfo.CreationParams.ParentWindow = nullptr;
	windowInfo.CreationParams.Menu = nullptr;
	windowInfo.CreationParams.LpParam = nullptr;
	windowInfo.DimParams = appMgrHandle->GetInitWindowDimParams();

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

	CRendererManager::Create(SRendererCreationInfo{ERenderAPIs::DX12, (uint32)appMgrHandle->GetInitWindowDimParams().Width, (uint32)appMgrHandle->GetInitWindowDimParams().Height, CMainWindow::GetWindowHandle()->GetHwnd()});

	CMainWindow::ShowAndUpdate(_nShowCmd);

	CRendererManager::OnInit(appMgrHandle->GetCurrentProject()->GetShaderPaths());

	ImGui::StyleColorsDark();


	MSG msg = {};
	bool b = true;
	while (msg.message != WM_QUIT)
	{
		CTimer::Update();
		if (CMainWindow::RunWindow(msg))
		{
			continue;
		}
		CGUI::NewFrame();

		if (b)
		{
			ImGui::ShowDemoWindow(&b);
		}

		CRendererManager::PrepareRendering();
		CGUI::Render();
		CRendererManager::OnRender();
	}

	CRendererManager::Destroy();
	CAppManager::Destroy();

	delete threadPool;

	return 0;
}