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

	PTimer::Create();

	PGuid guid = PGuid::NewGuid();
	std::string guidStr = guid.ToString();

	std::string errorMsg = {};
	if (!PLog::Init(errorMsg))
	{
#if defined(_DEBUG)
		PE_DEBUG_LOG(errorMsg);
#endif
		return -1;
	}
	CECSManager::CreateECS();

	if (!PAppManager::InitApp(_hInstance))
	{
		return -1;
	}

	PSavingSystem::OnCreate();

	PAppManager* appMgrHandle = PAppManager::GetAppHandle();

	PThreadPool* threadPool = new PThreadPool(std::thread::hardware_concurrency());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	PGUI::Init(PMainWindow::GetWindowHandle()->GetHwnd());

	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	u64 texID = 0;
	PRendererManager::CreateFontsTexture(pixels, width, height, texID);
	io.Fonts->TexID = (void*)texID;

	PMainWindow::ShowAndUpdate(_nShowCmd);

	ImGui::StyleColorsDark();


	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PMainWindow::RunWindow(msg))
		{
			continue;
		}
		PTimer::OnUpdate();
		PGUI::NewFrame();

		PRendererManager::PrepareRendering();
		PGUI::Render();
		PRendererManager::OnUpdate();
		PRendererManager::OnRender();
	}
	PAppManager::SetLastVSyncState(PRendererManager::GetVSync());
	PRendererManager::Destroy();
	PGUI::Destroy();
	PAppManager::Destroy();

	delete threadPool;

	return 0;
}