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

Protostar::PAppMenuBar::PAppMenuBar()
	:m_logWindow(nullptr)
	,m_editorFPS(nullptr)
	,m_bShowSaveWindow(false)
	,m_dirtyObjects(std::unordered_map<PBaseObject*, bool>())
{
	if (PGUI::GetGUIInfo().bOpenConsole)
	{
		m_logWindow = new PLogWindow();
	}
	if (PGUI::GetGUIInfo().bOpenEditorFPS)
	{
		m_editorFPS = new PEditorFPS();
	}
}

Protostar::PAppMenuBar::~PAppMenuBar()
{
}

void Protostar::PAppMenuBar::OnRender()
{

	if (!ImGui::BeginMainMenuBar())
	{
		return;
	}

	if(ImGui::BeginMenu("App"))
	{
		PAppManager* appManager = PAppManager::GetAppHandle();
		if (ImGui::BeginMenu("Open"))
		{
			for (PProject* project : appManager->GetProjects())
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
				m_logWindow = new PLogWindow();
			}
			PGUI::SetOpenLog(!PGUI::GetGUIInfo().bOpenConsole);
		}

		if (ImGui::MenuItem("Save...", "CTRL+S", false, PSavingSystem::GetHandle()->HasDirtyObjects()))
		{
			m_bShowSaveWindow = !m_bShowSaveWindow;			
		}

		if (ImGui::MenuItem("Save all", "CTRL+SHIFT+S", false, PSavingSystem::GetHandle()->HasDirtyObjects()))
		{
			PSavingSystem::GetHandle()->SaveAllDirtyObjects();
		}

		ImGui::Separator();

		if(ImGui::MenuItem("Content Browser"))
		{
			if (PProject* currentProject = appManager->GetCurrentProject())
			{
				PProjectGUI::GetProjectGUIHandle()->ToggleContentBrowser();
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
				m_editorFPS = new PEditorFPS();
			}
			PGUI::SetOpenEditorFPS(!PGUI::GetGUIInfo().bOpenEditorFPS);
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
			if (PSavingSystem::GetHandle()->HasDirtyObjects())
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
			PSavingSystem::GetHandle()->SaveAllDirtyObjects();
			bCtrlShiftAndSPressedLastFrame = false;
		}
	}

	ShowSaveWindow();

}

void Protostar::PAppMenuBar::ShowSaveWindow()
{
	static bool bIsInitialized = false;
	if (!m_bShowSaveWindow)
	{
		bIsInitialized = false;
		return;
	}

	if (!bIsInitialized)
	{
		for (PBaseObject* dirtyObject : PSavingSystem::GetHandle()->GetDirtyObjects())
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
			std::vector<PBaseObject*> objectsToSave = {};
			for (auto& [key, value] : m_dirtyObjects)
			{
				if (value)
				{
					objectsToSave.push_back(key);
				}
			}
			PSavingSystem::GetHandle()->SaveDirtyObjects(objectsToSave);

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
