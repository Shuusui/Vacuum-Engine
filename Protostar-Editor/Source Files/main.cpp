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
	CECSManager::CreateECS();

	if (!CAppManager::InitApp(_hInstance))
	{
		return -1;
	}

	CSavingSystem::OnCreate();

	CAppManager* appMgrHandle = CAppManager::GetAppHandle();

	CThreadPool* threadPool = new CThreadPool(std::thread::hardware_concurrency());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

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