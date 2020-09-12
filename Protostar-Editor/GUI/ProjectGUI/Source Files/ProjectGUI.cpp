#include "..\Header Files\ProjectGUI.h"
#include "..\Header Files\ContentBrowser.h"
#include "..\Header Files\EntityEditor.h"
#include "imgui.h"
#include "AppManager.h"
#include "SavingSystem.h"
#include "GlobalMacros.h"
#include "Util.h"
#include "GUI.h"
#include "RendererManager.h"
#include "MeshManager.h"

Protostar::CProjectGUI* Protostar::CProjectGUI::s_projectGUI = nullptr;

void Protostar::CProjectGUI::ToggleContentBrowser()
{
	CGUI::SetOpenContentBrowser(!CGUI::GetGUIInfo().bOpenContentBrowser);
}

Protostar::CProjectGUI::CProjectGUI()
	:m_currentProject(nullptr)
	,m_savingSystem(nullptr)
	,m_contentBrowser(nullptr)
	,m_entityEditor(nullptr)
	,m_setProjectFuncIndex(0)
	,m_bShowCreateSceneWindow(false)
{
	memset(m_newSceneName, 0, sizeof(m_newSceneName));
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
	m_savingSystem = CSavingSystem::GetHandle();

	m_contentBrowser = new CContentBrowser(CAppManager::GetAppHandle()->GetCurrentProject());

	m_entityEditor = CEntityEditor::OnCreate();
}

void Protostar::CProjectGUI::OnCreate()
{
	if (s_projectGUI)
	{
		return;
	}
	s_projectGUI = new CProjectGUI();
}

void Protostar::CProjectGUI::OnDestroy()
{
	if (s_projectGUI)
	{
		delete s_projectGUI; 
		s_projectGUI = nullptr;
	}
}

Protostar::CProjectGUI::~CProjectGUI()
{
	CAppManager::GetAppHandle()->RemoveOnLoadProjectCallback(m_setProjectFuncIndex);
}

void Protostar::CProjectGUI::OnRender()
{
	if (!s_projectGUI)
	{
		return;
	}

	if (!m_currentProject)
	{
		m_currentProject = CAppManager::GetAppHandle()->GetCurrentProject();
		if(!m_currentProject)
		{
			return;
		}
	}

	if (m_contentBrowser && CGUI::GetGUIInfo().bOpenContentBrowser)
	{
		m_contentBrowser->OnRender();
	}

	if (m_entityEditor)
	{
		m_entityEditor->OnRender();
	}
	RenderViewport();

	RenderCreateSceneWindow();

	RenderCreateEntityWindow();
}

void Protostar::CProjectGUI::RenderViewport()
{
	std::string wndLable = m_currentProject->GetCurrentScene() ? PRINTF("%s (%s)", m_currentProject->GetName().c_str(), m_currentProject->GetCurrentScene()->GetObjectName().c_str()).c_str() : m_currentProject->GetName();

	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(wndLable.c_str()))
	{
		ImGui::End();
		return;
	}

	ImGui::InvisibleButton("Viewport", ImGui::GetContentRegionAvail(), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

	ImVec2 viewportPos = ImGui::GetWindowPos();
	ImVec2 viewportSize = ImGui::GetWindowSize();

	CMeshManager* meshManager = CMeshManager::GetHandle();
	std::unordered_map<SGuid, SModel> meshes = meshManager->GetMeshes();
	SModel currentModel = {};
	for (const auto& [guid, model] : meshes)
	{
		currentModel = model;
		break;
	}

	SMesh meshData = currentModel.MeshData;

	SDrawData* drawData = new SDrawData();
	drawData->DisplayPos = DirectX::XMFLOAT2{viewportPos.x, viewportPos.y};
	drawData->DisplaySize = DirectX::XMFLOAT2{viewportSize.x, viewportSize.y};
	drawData->TotalIdxCount = meshData.Indices.size();
	drawData->TotalVtxCount = meshData.Vertices.size();

	SDrawList drawList = {};
	drawList.IndexBuffer = meshData.Indices;
	drawList.VertexBuffer = meshData.Vertices;

	SDrawCmd drawCmd = {};
	drawCmd.IdxOffset = 0;
	drawCmd.VtxOffset = 0;
	drawCmd.ElemCount = meshData.Indices.size();
	drawCmd.ClipRect = DirectX::XMFLOAT4{0, 0, viewportSize.x, viewportSize.y};

	drawList.DrawCmds = std::vector<SDrawCmd>{drawCmd};
	drawData->DrawLists = std::vector<SDrawList>{drawList};

	CRendererManager::AddDrawData(drawData);

	ImGui::End();
}

void Protostar::CProjectGUI::RenderCreateSceneWindow()
{
	if (!m_bShowCreateSceneWindow)
	{
		return;
	}
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * .5f, io.DisplaySize.y * .5f));
	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Create scene", &m_bShowCreateSceneWindow))
	{
		ImGui::End();
		return;
	}
	ImGui::InputText("Scene name ", m_newSceneName, IM_ARRAYSIZE(m_newSceneName));
	if (ImGui::Button("Create"))
	{
		m_currentProject->CreateScene(m_newSceneName);
		m_bShowCreateSceneWindow = false;
		memset(m_newSceneName, 0, sizeof(m_newSceneName));
	}
	ImGui::End();
}

void Protostar::CProjectGUI::RenderCreateEntityWindow()
{
	if (!m_bShowCreateEntityWindow)
	{
		return;
	}
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * .5f, io.DisplaySize.y * .5f));
	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Create entity", &m_bShowCreateEntityWindow))
	{
		ImGui::End();
		return;
	}
	ImGui::InputText("Entity name ", m_newEntityName, IM_ARRAYSIZE(m_newEntityName));
	if (ImGui::Button("Create"))
	{
		m_currentProject->CreateEntity(m_newEntityName);
		m_bShowCreateEntityWindow = false;
		memset(m_newEntityName, 0, sizeof(m_newEntityName));
	}
	ImGui::End();
}
