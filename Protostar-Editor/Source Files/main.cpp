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
#include "SavingSystem.h"
#include "SharedStructs.h"
#include "RendererManager.h"
#include "imgui.h"
#include "GUI.h"
#include "Timer.h"
#include "ECS\Header Files\ECSManager.h"

s32 WinMain(_In_ HINSTANCE _hInstance, _In_opt_  HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ s32 _nShowCmd)
{
	using namespace Protostar;

	CTimer::Create();

	SGuid guid = SGuid::NewGuid();
	std::string guidStr = guid.ToString();

	std::string errorMsg = {};
	if (!CLog::Init(errorMsg))
	{
#if defined(_DEBUG)
		PE_DEBUG_LOG(errorMsg);
#endif
		return -1;
	}

	CAppManager::InitApp();

	CAppManager* appMgrHandle = CAppManager::GetAppHandle();

	CECSManager::CreateECS();

	CSavingSystem::OnCreate();

	CThreadPool* threadPool = new CThreadPool(std::thread::hardware_concurrency());

	SWindowInfo windowInfo = {};
	windowInfo.ClassParams.ClassName = TEXT("Protostar Engine window");
	windowInfo.ClassParams.HInstance = _hInstance;
	windowInfo.ClassParams.BackgroundColor = CreateSolidBrush(RGB(1, 1, 1));
	windowInfo.ClassParams.Style = CS_HREDRAW | CS_VREDRAW;
	windowInfo.CreationParams.DwExStyle = NULL;
	windowInfo.CreationParams.DwStyle = WS_OVERLAPPEDWINDOW;
	windowInfo.CreationParams.WindowName = TEXT("Protostar-Engine");
	windowInfo.CreationParams.ParentWindow = nullptr;
	windowInfo.CreationParams.Menu = nullptr;
	windowInfo.CreationParams.LpParam = nullptr;
	windowInfo.DimParams = appMgrHandle->GetInitWindowDimParams();

	CMainWindow::InitWindow(windowInfo);
	if (!CMainWindow::Create(errorMsg))
	{
		PE_LOG(errorMsg.c_str());
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	CRendererManager::Create(SRendererCreationInfo{ ERenderAPIs::DX12, (u32)appMgrHandle->GetInitWindowDimParams().Width, (u32)appMgrHandle->GetInitWindowDimParams().Height, appMgrHandle->GetLastVSyncState(), CMainWindow::GetWindowHandle()->GetHwnd() });

	CGUI::Init(CMainWindow::GetWindowHandle()->GetHwnd());

	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	u64 texID = 0;
	CRendererManager::CreateFontsTexture(pixels, width, height, texID);
	io.Fonts->TexID = (void*)texID;

	CMainWindow::ShowAndUpdate(_nShowCmd);

	ImGui::StyleColorsDark();


	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (CMainWindow::RunWindow(msg))
		{
			continue;
		}
		CTimer::OnUpdate();
		CGUI::NewFrame();

		CRendererManager::PrepareRendering();
		CGUI::Render();
		CRendererManager::OnUpdate();
		CRendererManager::OnRender();
	}
	CAppManager::SetLastVSyncState(CRendererManager::GetVSync());
	CRendererManager::Destroy();
	CGUI::Destroy();
	CAppManager::Destroy();

	delete threadPool;

	return 0;
}