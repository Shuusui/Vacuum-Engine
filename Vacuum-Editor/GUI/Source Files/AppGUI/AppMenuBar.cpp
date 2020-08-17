#include "..\..\Header Files\AppGUI\AppMenuBar.h"
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
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}
