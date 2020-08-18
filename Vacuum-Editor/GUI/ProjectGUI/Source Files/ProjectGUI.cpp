#include "..\Header Files\ProjectGUI.h"
#include "imgui.h"
#include "AppManager.h"

Vacuum::CProjectGUI::CProjectGUI()
	:m_currentProject(nullptr)
	,m_setProjectFuncIndex(0)
{
	CAppManager* appManager = CAppManager::GetAppHandle();
	if (!appManager)
	{
		return;
	}
	m_setProjectFuncIndex = appManager->RegisterOnLoadProjectCallback([this](CProject* _oldProject, CProject* _newProject)->void
		{
			if (!_newProject)
			{
				return;
			}
			m_currentProject = _newProject;
		});

	CProject* currentProject = appManager->GetCurrentProject();
	if (!currentProject)
	{
		return;
	}
	m_currentProject = currentProject;
}

Vacuum::CProjectGUI::~CProjectGUI()
{
	CAppManager::GetAppHandle()->RemoveOnLoadProjectCallback(m_setProjectFuncIndex);
}

void Vacuum::CProjectGUI::OnRender()
{
	if (!m_currentProject)
	{
		return;
	}
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(m_currentProject->GetName().c_str()))
	{
		return;
	}

	ImGui::End();
}
