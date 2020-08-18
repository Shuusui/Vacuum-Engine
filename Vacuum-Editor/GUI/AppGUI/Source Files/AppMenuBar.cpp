#include "..\Header Files\AppMenuBar.h"
#include "..\Header Files\LogWindow.h"
#include "imgui.h"
#include "AppManager.h"

Vacuum::CAppMenuBar::CAppMenuBar()
{

}

Vacuum::CAppMenuBar::~CAppMenuBar()
{

}

void Vacuum::CAppMenuBar::OnRender()
{
	if (!ImGui::BeginMainMenuBar())
	{
		return;
	}

	if(ImGui::BeginMenu("App"))
	{
		if (ImGui::BeginMenu("Open"))
		{
			CAppManager* appManager = CAppManager::GetAppHandle();
			for (CProject* project : appManager->GetProjects())
			{
				if (ImGui::MenuItem(project->GetName().c_str()))
				{
					appManager->LoadProject(project);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("LogWindow", "CTRL+L"))
		{
			m_logWindow = new CLogWindow();
			CAppManager* appManager = CAppManager::GetAppHandle();
			SWindowDimParams wndDim = appManager->GetInitWindowDimParams();
			int32 width = wndDim.Width - 40;
			int32 height = wndDim.Height *.15f;
			int32 x = 10;
			int32 y = wndDim.Height * .8f;
			ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
		}
		else
		{
			if (m_logWindow)
			{
				delete m_logWindow;
				m_logWindow = nullptr;
			}
		}
		ImGui::EndMenu();
	}
	if (m_logWindow)
	{
		m_logWindow->OnRender();
	}

	ImGui::EndMainMenuBar();
}
