#include "..\Header Files\ContentBrowser.h"
#include "..\Header Files\ProjectGUI.h"
#include "..\Header Files\EntityEditor.h"

#include "MeshManager.h"
#include "imgui.h"
#include "GUI.h"
#include "AppManager.h"
#include "Log.h"

Vacuum::CContentBrowser::CContentBrowser(CProject* _project)
	:m_project(_project)
	,m_meshManager(nullptr)
	,m_bDelKeyPressedThisFrame(false)
	,m_bRefresh(false)
	,m_scenes({})
	,m_entities({})
{
	if (!m_project)
	{
		return;
	}

	CheckPaths();
	m_meshManager = CMeshManager::GetHandle();
}

void Vacuum::CContentBrowser::OnRender()
{
	if (!CGUI::GetGUIInfo().bOpenContentBrowser)
	{
		return;
	}

	CAppManager* appManager = CAppManager::GetAppHandle();
	if (!appManager)
	{
		ImGui::End();
		return;
	}
	SWindowDimParams wndDim = appManager->GetInitWindowDimParams();
	float width = (float)wndDim.Width * .2f;
	float height = (float)wndDim.Height * .8f;
	float x = (float)wndDim.Width - width - 50;
	float y = (float)wndDim.Height - (wndDim.Height * .8f);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Content Browser", &CGUI::GetGUIInfo().bOpenContentBrowser, ImGuiWindowFlags_MenuBar))
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::Button("Refresh"))
		{
			m_bRefresh = true;
		}

		ImGui::EndMenuBar();
	}

	CProject* currentProject = appManager->GetCurrentProject();
	if (!m_project && !currentProject)
	{
		ImGui::End();
		return;
	}
	if (!m_project)
	{
		m_project = currentProject;
		m_meshManager = CMeshManager::GetHandle();
	}

	if (currentProject != m_project)
	{
		m_project = currentProject;
		CheckPaths();
	}

	if (ImGui::IsKeyPressed(VK_DELETE))
	{
		m_bDelKeyPressedThisFrame = true;
	}

	
	ManageShaderPaths();

	ImGui::Separator();

	ManageModelPaths();

	ImGui::Separator();

	ManageScenePaths();

	ImGui::Separator();

	ShowEntities();

	ImGui::Separator();

	DisplayContextMenu();

	ImGui::End();

	m_bRefresh = false;
}

void Vacuum::CContentBrowser::DisplayContextMenu()
{
	if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_NoOpenOverExistingPopup))
	{
		if (ImGui::MenuItem("Create new scene"))
		{
			if (CProjectGUI* projectGUI = CProjectGUI::GetProjectGUIHandle())
			{
				projectGUI->SetShowCreateSceneWindow(true);
			}
		}

		if (ImGui::MenuItem("Create new entity"))
		{
			if (CProjectGUI* projectGUI = CProjectGUI::GetProjectGUIHandle())
			{
				projectGUI->SetShowCreateEntityWindow(true);
			}
		}

		ImGui::EndPopup();
	}
}

void Vacuum::CContentBrowser::ManageShaderPaths()
{
	if (!ImGui::TreeNode("Shaders"))
	{
		return;
	}
	
	std::unordered_map<std::string, SShaderInfo> shaders = m_project->GetShaders();

	for (const std::filesystem::path& shaderPath : std::filesystem::directory_iterator(m_project->GetProjectPaths().ShaderDir))
	{
		if (shaderPath.extension() != ".hlsl")
		{
			continue;
		}
		if (shaders.find(shaderPath.string()) != shaders.end())
		{
			continue;
		}
		m_project->RegisterShader(shaderPath);
	}

	for (const std::pair<std::string, SShaderInfo>& shader : shaders)
	{
		if (!std::filesystem::exists(shader.first))
		{
			m_project->RemoveShader(shader.first);
		}

		ImGui::Selectable(shader.second.ShaderName.c_str());
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Path: %s", shader.first.c_str());
			ImGui::EndTooltip();
		}
	}

	ImGui::TreePop();
}

void Vacuum::CContentBrowser::ManageModelPaths()
{
	if (!ImGui::TreeNode("Models"))
	{
		return;
	}

	std::unordered_map<SGuid, SModel> models = m_meshManager->GetMeshes();
	std::unordered_set<std::string> modelPaths = m_meshManager->GetMeshPaths();

	for (const std::filesystem::path& modelPath : std::filesystem::directory_iterator(m_project->GetProjectPaths().ModelsDir))
	{
		std::filesystem::path extension = modelPath.extension();
		if (extension != ".obj" && extension != ".fbx")
		{
			continue;
		}
		
		if (modelPaths.find(modelPath.string()) != modelPaths.end())
		{
			continue;
		}

		m_meshManager->RegisterModel(modelPath);
	}

	for (const auto& [guid, model] : models)
	{
		if (!std::filesystem::exists(model.Path))
		{
			m_meshManager->UnregisterModel(guid, model.Path.string());
		}

		ImGui::Selectable(model.Name.c_str());
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Path: %s", model.Path.string().c_str());
			ImGui::EndTooltip();
		}
	}
	ImGui::TreePop();
}

void Vacuum::CContentBrowser::ManageScenePaths()
{
	static bool bIsInitialized = false;
	if (!ImGui::TreeNode("Scenes"))
	{
		bIsInitialized = false;
		return;
	}

	if (m_scenes.size() != m_project->GetScenes().size() || m_bRefresh)
	{
		bIsInitialized = false;
	}

	if (!bIsInitialized)
	{
		InitSceneTreeNode();
		bIsInitialized = true;
	}

	bool bDoubleClicked = false;
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		bDoubleClicked = true;
	}

	for (auto& [key, value] : m_scenes)
	{
		if (ImGui::Selectable(key->GetObjectName().c_str(), value))
		{
			value = !value;
		}
		if (ImGui::IsItemHovered())
		{
			if (bDoubleClicked)
			{
				if (key != m_project->GetCurrentScene())
				{
					m_project->LoadScene(key);
				}
			}
		}
		if (m_bDelKeyPressedThisFrame && value)
		{
			m_project->DeleteScene(key);
			bIsInitialized = false;
		}
	}

	ImGui::TreePop();
}

void Vacuum::CContentBrowser::ShowEntities()
{
	static bool bIsInitialized = false;
	if (!ImGui::TreeNode("Entities"))
	{
		bIsInitialized = false;
		return;
	}

	if (m_bRefresh || m_entities.size() != CEntityManager::GetHandle()->GetEntities().size())
	{
		bIsInitialized = false;
	}

	if (!bIsInitialized)
	{
		for (CBaseEntity* entity : CEntityManager::GetHandle()->GetEntities())
		{
			m_entities.insert(std::make_pair(entity, false));
		}
		bIsInitialized = true;
	}

	for (auto& [key, value] : m_entities)
	{
		ImGui::Selectable(key->GetObjectName().c_str(), &value);
		ImGui::Indent(3.0f);
		if (ImGui::BeginPopupContextItem(nullptr, ImGuiMouseButton_Right))
		{
			if (ImGui::Button("Open entity editor"))
			{
				CProjectGUI::GetProjectGUIHandle()->GetEntityEditorHandle()->OpenEditor(key);
			}
			ImGui::EndPopup();
		}
	}

	ImGui::TreePop();
}

void Vacuum::CContentBrowser::RenderComponents()
{
	if (!ImGui::TreeNode("Components"))
	{
		return;
	}

	ImGui::TreePop();
}

void Vacuum::CContentBrowser::CheckPaths()
{
	if (!std::filesystem::exists(m_project->GetProjectPaths().ShaderDir))
	{
		std::filesystem::create_directories(m_project->GetProjectPaths().ShaderDir);
	}

	if (!std::filesystem::exists(m_project->GetProjectPaths().ModelsDir))
	{
		std::filesystem::create_directories(m_project->GetProjectPaths().ModelsDir);
	}

	if (!std::filesystem::exists(m_project->GetProjectPaths().ScenesDir))
	{
		std::filesystem::create_directories(m_project->GetProjectPaths().ScenesDir);
	}
}

void Vacuum::CContentBrowser::InitSceneTreeNode()
{
	m_scenes.clear();
	for (CScene* scene : m_project->GetScenes())
	{
		m_scenes.insert(std::make_pair(scene, false));
	}
}