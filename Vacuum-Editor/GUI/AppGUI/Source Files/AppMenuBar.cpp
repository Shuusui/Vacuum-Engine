#include "..\Header Files\AppMenuBar.h"
#include "..\Header Files\LogWindow.h"
#include "imgui.h"
#include "AppManager.h"
#include "GUI.h"

Vacuum::CAppMenuBar::CAppMenuBar()
	:m_logWindow(nullptr)
{
	if (CGUI::GetGUIInfo().bOpenConsole)
	{
		m_logWindow = new CLogWindow();
	}
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
			if (!m_logWindow)
			{
				m_logWindow = new CLogWindow();
			}
			CGUI::SetOpenLog(!CGUI::GetGUIInfo().bOpenConsole);
		}
		ImGui::EndMenu();
	}
	if (m_logWindow)
	{
		m_logWindow->OnRender();
	}

	ImGui::EndMainMenuBar();
}
