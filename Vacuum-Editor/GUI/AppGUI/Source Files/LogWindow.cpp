#include "..\Header Files\LogWindow.h"
#include "imgui.h"
#include "Log.h"
#include "AppManager.h"
#include "GUI.h"

Vacuum::CLogWindow::CLogWindow()
	:m_logBuffer(std::vector<std::pair<SColor, std::string>>())
	,m_guid(SGuid::NewGuid())
	,m_lastFrameVectorSize(0)
	,m_bStopScroll(false)
{
	RegisterToLog();
}

Vacuum::CLogWindow::~CLogWindow()
{
	if (CLog::IsBufRegistered(m_guid))
	{
		CLog::RemoveBuffer(m_guid);
	}
}

void Vacuum::CLogWindow::OnRender()
{
	if (!CGUI::GetGUIInfo().bOpenConsole)
	{
		return;
	}

	CAppManager* appManager = CAppManager::GetAppHandle();
	SWindowDimParams wndDim = appManager->GetInitWindowDimParams();
	float width = (float)wndDim.Width - 40;
	float height = (float)wndDim.Height * .15f;
	float x = 10.0f;
	float y = (float)wndDim.Height * .8f;
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Log", &CGUI::GetGUIInfo().bOpenConsole))
	{ 
		if (CLog::IsBufRegistered(m_guid))
		{
			CLog::RemoveBuffer(m_guid);
		}

		ImGui::End();
		return;
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		if (ImGui::IsWindowHovered())
		{
			m_bStopScroll = !m_bStopScroll;
		}
	}
	bool bScroll = false;
	if (!m_bStopScroll && m_lastFrameVectorSize != m_logBuffer.size())
	{
		m_lastFrameVectorSize = m_logBuffer.size();
		bScroll = true;
	}
	for (const std::pair<SColor, std::string>& bufInformation : m_logBuffer)
	{
		ImGui::TextColored(ImVec4(bufInformation.first.Color[0], bufInformation.first.Color[1], bufInformation.first.Color[2], bufInformation.first.Color[3]), bufInformation.second.c_str());
		if (bScroll)
		{
			ImGui::SetScrollHereY();
		}
	}
	ImGui::End();
}

void Vacuum::CLogWindow::RegisterToLog()
{
	CLog::RegisterBuffer(m_guid, &m_logBuffer);
}
