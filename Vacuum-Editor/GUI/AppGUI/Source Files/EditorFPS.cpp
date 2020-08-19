#include "..\Header Files\EditorFPS.h"
#include "AppManager.h"
#include "GUI.h"
#include "imgui.h"
#include "Timer.h"

Vacuum::CEditorFPS::CEditorFPS()
{
}

Vacuum::CEditorFPS::~CEditorFPS()
{
}

void Vacuum::CEditorFPS::OnRender()
{
	if (!CGUI::GetGUIInfo().bOpenEditorFPS)
	{
		return;
	}

	CAppManager* appManager = CAppManager::GetAppHandle();
	SWindowDimParams wndDim = appManager->GetInitWindowDimParams();
	int32 x = wndDim.Width - 200;
	int32 y = wndDim.Height * .1f;
	ImGui::SetNextWindowSize(ImVec2(160, 80), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Editor FPS", &CGUI::GetGUIInfo().bOpenEditorFPS))
	{
		return; 
	}


	ImGui::End();
}
