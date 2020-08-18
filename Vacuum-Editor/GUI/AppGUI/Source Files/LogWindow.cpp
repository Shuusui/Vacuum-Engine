#include "..\Header Files\LogWindow.h"
#include "imgui.h"
#include "Log.h"

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
	if (!ImGui::Begin("Log"))
	{
		if (CLog::IsBufRegistered(m_guid))
		{
			CLog::RemoveBuffer(m_guid);
		}
		return;
	}
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		if (ImGui::IsItemHovered())
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
			ImGui::SetScrollHereY(1.0f);
		}
	}
	ImGui::End();
}

void Vacuum::CLogWindow::RegisterToLog()
{
	CLog::RegisterBuffer(m_guid, &m_logBuffer);
}
