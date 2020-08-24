#include "..\Header Files\AppMenuBar.h"
#include "..\Header Files\LogWindow.h"
#include "..\Header Files\EditorFPS.h"
#include "..\..\ProjectGUI\Header Files\ProjectGUI.h"
#include "Project.h"
#include "imgui.h"
#include "AppManager.h"
#include "GUI.h"
#include "Log.h"
#include "SavingSystem.h"
#include "ECS\Header Files\BaseObject.h"

Vacuum::CAppMenuBar::CAppMenuBar()
	:m_logWindow(nullptr)
	,m_editorFPS(nullptr)
	,m_bShowSaveWindow(false)
	,m_dirtyObjects(std::unordered_map<CBaseObject*, bool>())
{
	if (CGUI::GetGUIInfo().bOpenConsole)
	{
		m_logWindow = new CLogWindow();
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

		if (ImGui::MenuItem("Save...", "CTRL+S", false, CSavingSystem::GetHandle()->HasDirtyObjects()))
		{
			m_bShowSaveWindow = !m_bShowSaveWindow;			
		}

		if (ImGui::MenuItem("Save all", "CTRL+SHIFT+S", false, CSavingSystem::GetHandle()->HasDirtyObjects()))
		{
			CSavingSystem::GetHandle()->SaveAllDirtyObjects();
		}

		ImGui::Separator();

		if(ImGui::MenuItem("Content Browser"))
		{
			if (CProject* currentProject = appManager->GetCurrentProject())
			{
				CProjectGUI::GetProjectGUIHandle()->ToggleContentBrowser();
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
	ImGui::EndMainMenuBar();

	if (m_logWindow)
	{
		m_logWindow->OnRender();
	}

	if (m_editorFPS)
	{
		m_editorFPS->OnRender();
	}

	ImGuiIO& io = ImGui::GetIO();

	static bool bCtrlAndSPressedLastFrame = false;

	if (io.KeysDown[VK_CONTROL] && io.KeysDown[0x53])
	{
		bCtrlAndSPressedLastFrame = true;
	}

	if (bCtrlAndSPressedLastFrame)
	{
		if (!io.KeysDown[VK_CONTROL] && !io.KeysDown[0x53])
		{
			if (CSavingSystem::GetHandle()->HasDirtyObjects())
			{
				m_bShowSaveWindow = !m_bShowSaveWindow;
			}
			bCtrlAndSPressedLastFrame = false;
		}
	}

	static bool bCtrlShiftAndSPressedLastFrame = false;
	if (io.KeysDown[VK_CONTROL] && io.KeysDown[VK_SHIFT] && io.KeysDown[0x53])
	{
		bCtrlShiftAndSPressedLastFrame = true;
	}

	if (bCtrlShiftAndSPressedLastFrame)
	{
		if (!io.KeysDown[VK_CONTROL] && !io.KeysDown[VK_SHIFT] && !io.KeysDown[0x53])
		{
			CSavingSystem::GetHandle()->SaveAllDirtyObjects();
			bCtrlShiftAndSPressedLastFrame = false;
		}
	}

	ShowSaveWindow();

}

void Vacuum::CAppMenuBar::ShowSaveWindow()
{
	static bool bIsInitialized = false;
	if (!m_bShowSaveWindow)
	{
		bIsInitialized = false;
		return;
	}

	if (!bIsInitialized)
	{
		for (CBaseObject* dirtyObject : CSavingSystem::GetHandle()->GetDirtyObjects())
		{
			m_dirtyObjects.insert(std::make_pair(dirtyObject, true));
		}
		bIsInitialized = true;
	}

	if(!ImGui::IsPopupOpen("Save"))
	{
		ImGui::OpenPopup("Save");
	}

	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(200,0));

	if (ImGui::BeginPopupModal("Save", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::Button("Select all"))
		{
			for (auto& [key, value] : m_dirtyObjects)
			{
				value = true;
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Deselect all"))
		{
			for (auto& [key, value] : m_dirtyObjects)
			{
				value = false;
			}
		}

		ImGui::Separator();

		for (auto& [key, value] : m_dirtyObjects)
		{
			ImGui::Text(key->GetObjectName().c_str());
			ImGui::SameLine();
			ImGui::Checkbox("Save?", &value);
		}

		ImGui::Separator();

		if (ImGui::Button("Ok"))
		{
			std::vector<CBaseObject*> objectsToSave = {};
			for (auto& [key, value] : m_dirtyObjects)
			{
				if (value)
				{
					objectsToSave.push_back(key);
				}
			}
			CSavingSystem::GetHandle()->SaveDirtyObjects(objectsToSave);

			ImGui::CloseCurrentPopup();
			m_bShowSaveWindow = false;
			m_dirtyObjects.clear();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			m_bShowSaveWindow = false;
			m_dirtyObjects.clear();
		}

		ImGui::EndPopup();
	}
}
