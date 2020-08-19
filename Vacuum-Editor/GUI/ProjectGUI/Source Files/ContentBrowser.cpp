#include "..\Header Files\ContentBrowser.h"
#include "imgui.h"
#include "GUI.h"
#include "AppManager.h"

Vacuum::CContentBrowser::CContentBrowser(const std::filesystem::path& _contentDir)
	:m_contentDir(_contentDir)
	,m_shaderDir(_contentDir / "Shaders")
	,m_modelsDir(_contentDir / "Models")
{
}

void Vacuum::CContentBrowser::OnRender()
{
	if (!CGUI::GetGUIInfo().bOpenContentBrowser)
	{
		return;
	}

	CAppManager* appManager = CAppManager::GetAppHandle();
	SWindowDimParams wndDim = appManager->GetInitWindowDimParams();
	int32 width = wndDim.Width * .2f;
	int32 height = wndDim.Height * .8f;
	int32 x = wndDim.Width - width -50;
	int32 y = wndDim.Height - (wndDim.Height * .8f);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Content Browser", &CGUI::GetGUIInfo().bOpenContentBrowser))
	{
		return;
	}
	if (!std::filesystem::exists(m_shaderDir))
	{
		std::filesystem::create_directories(m_shaderDir);
	}

	for (const std::filesystem::path& shaderPath : std::filesystem::directory_iterator(m_shaderDir))
	{
		bool bFoundShader = false;
		for (const SShaderInfo& shader : m_shaders)
		{
			if (shader.ShaderPath != shaderPath)
			{
				continue;
			}
			bFoundShader = true;
			break;
		}
		if (!bFoundShader)
		{
			m_shaders.push_back(shaderPath);
		}
	}
	if (m_shaders.size() > 0)
	{
		m_shaders.erase(std::remove_if(m_shaders.begin(), m_shaders.end(), [](const SShaderInfo& _shaderInfo)->bool{return !std::filesystem::exists(_shaderInfo.ShaderPath);}), m_shaders.end());
	}

	if (ImGui::TreeNode("Shaders"))
	{
		for (const SShaderInfo& shader : m_shaders)
		{
			ImGui::Selectable(shader.ShaderPath.filename().generic_string().c_str(), &shader.bIsSelected);
		}
		ImGui::TreePop();
	}

	if (!std::filesystem::exists(m_modelsDir))
	{
		std::filesystem::create_directories(m_modelsDir);
	}

	ImGui::End();
}
