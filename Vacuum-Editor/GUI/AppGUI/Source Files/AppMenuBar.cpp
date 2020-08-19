#include "..\Header Files\AppMenuBar.h"
#include "..\Header Files\LogWindow.h"
#include "..\Header Files\EditorFPS.h"
#include "..\..\ProjectGUI\Header Files\ContentBrowser.h"
#include "Project.h"
#include "imgui.h"
#include "AppManager.h"
#include "GUI.h"

Vacuum::CAppMenuBar::CAppMenuBar()
	:m_logWindow(nullptr)
	,m_contentBrowser(nullptr)
{
	if (CGUI::GetGUIInfo().bOpenConsole)
	{
		m_logWindow = new CLogWindow();
	}
	if (CGUI::GetGUIInfo().bOpenContentBrowser)
	{
		m_contentBrowser = new CContentBrowser(CAppManager::GetAppHandle()->GetCurrentProject()->GetProjectPaths().ContentDir);
	}
	if (CGUI::GetGUIInfo().bOpenEditorFPS)
	{
		m_editorFPS = new CEditorFPS();
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
		CAppManager* appManager = CAppManager::GetAppHandle();
		if (ImGui::BeginMenu("Open"))
		{
			for (CProject* project : appManager->GetProjects())
			{
				if (ImGui::MenuItem(project->GetName().c_str()))
				{
					appManager->LoadProject(project);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Log Window", "CTRL+L"))
		{
			if (!m_logWindow)
			{
				m_logWindow = new CLogWindow();
			}
			CGUI::SetOpenLog(!CGUI::GetGUIInfo().bOpenConsole);
		}

		ImGui::Separator();

		if(ImGui::MenuItem("Content Browser"))
		{
			if (CProject* currentProject = appManager->GetCurrentProject())
			{
				if (!m_contentBrowser)
				{
					m_contentBrowser = new CContentBrowser(currentProject->GetProjectPaths().ContentDir);
				}
				CGUI::SetOpenContentBrowser(!CGUI::GetGUIInfo().bOpenContentBrowser);
			}
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Extra Windows"))
	{
		if (ImGui::MenuItem("Editor FPS"))
		{
			if (!m_editorFPS)
			{
				m_editorFPS = new CEditorFPS();
			}
			CGUI::SetOpenEditorFPS(!CGUI::GetGUIInfo().bOpenEditorFPS);
		}
		ImGui::EndMenu();
	}

	if (m_logWindow)
	{
		m_logWindow->OnRender();
	}

	if (m_contentBrowser)
	{
		m_contentBrowser->OnRender();
	}

	if (m_editorFPS)
	{
		m_editorFPS->OnRender();
	}

	ImGui::EndMainMenuBar();
}
