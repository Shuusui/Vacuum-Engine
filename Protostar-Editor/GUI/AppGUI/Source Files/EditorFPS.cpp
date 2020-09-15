#include "..\Header Files\EditorFPS.h"
#include "AppManager.h"
#include "GUI.h"
#include "imgui.h"
#include "Timer.h"
#include "RendererManager.h"

Protostar::CEditorFPS::CEditorFPS()
{
}

Protostar::CEditorFPS::~CEditorFPS()
{
}

void Protostar::CEditorFPS::OnRender()
{
	if (!CGUI::GetGUIInfo().bOpenEditorFPS)
	{
		return;
	}

	CAppManager* appManager = CAppManager::GetAppHandle();
	SWindowDimParams wndDim = appManager->GetInitWindowDimParams();
	float x = (float)wndDim.Width - 200;
	float y = (float)wndDim.Height * .1f;
	ImGui::SetNextWindowSize(ImVec2(160.0f, 80.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Editor FPS", &CGUI::GetGUIInfo().bOpenEditorFPS))
	{
		ImGui::End();
		return;
	}

	ImGui::Checkbox("VSync", &CRendererManager::GetVSync());

	ImGui::Text("%.3f ms/frame", CTimer::GetDeltaSeconds());
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);


	ImGui::End();
}
