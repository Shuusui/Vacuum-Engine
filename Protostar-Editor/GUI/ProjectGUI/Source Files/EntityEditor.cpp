#include "..\Header Files\EntityEditor.h"
#include "imgui.h"
#include <string>
#include "ECS\Entities\Header Files\BaseEntity.h"
#include "ECS\Header Files\ECSManager.h"
#include "ECS\Components\Header Files\TransformComponent.h"
#include "ECS\Components\Header Files\MeshComponent.h"
#include "SavingSystem.h"
#include "MeshManager.h"

Protostar::PEntityEditor* Protostar::PEntityEditor::s_entityEditor = nullptr;

enum class ESelected
{
	None,
	Transform, 
	Mesh
};

Protostar::PEntityEditor* Protostar::PEntityEditor::OnCreate()
{
	if (s_entityEditor)
	{
		return s_entityEditor;
	}

	return s_entityEditor = new PEntityEditor();
}

void Protostar::PEntityEditor::OpenEditor(PBaseEntity* _entity)
{
	m_entity = _entity;
	m_bEntityEditorOpen = true;
}

void Protostar::PEntityEditor::OnRender()
{
	if (!m_entity || !m_bEntityEditorOpen)
	{
		m_bEntityEditorOpen = false;
		return;
	}

	ImVec2 windowPos = ImGui::GetWindowPos();
	ImGui::SetWindowPos(ImVec2(windowPos.x * 0.5f, windowPos.y * 0.5f), ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
	if(!ImGui::Begin("Entity Editor", &m_bEntityEditorOpen))
	{
		ImGui::End();
		return;
	}

	entt::registry& registry = CECSManager::GetRegistry();
	entt::entity& entity = m_entity->GetEntity();

	static ESelected selected = ESelected::None;

	if (ImGui::BeginChild("Overview", ImVec2(300, 0), true))
	{
		static char* textBuf = _strdup(m_entity->GetObjectName().c_str());
		ImGui::Text("Overview:");
		if (ImGui::InputText("Name", textBuf, IM_ARRAYSIZE(textBuf)))
		{
			m_entity->SetObjectName(textBuf);
			PSavingSystem::GetHandle()->RegisterDirtyObject(m_entity);
		}

		if (registry.has<CTransformComponent>(entity))
		{
			if (ImGui::Selectable("Transform Component", selected == ESelected::Transform))
			{
				selected = ESelected::Transform;
			}
		}

		if (registry.has<CMeshComponent>(entity))
		{
			if (ImGui::Selectable("Mesh Component", selected == ESelected::Mesh))
			{
				selected = ESelected::Mesh;
			}
		}

		if (ImGui::BeginCombo("Add Component", nullptr, ImGuiComboFlags_NoPreview))
		{
			if (!registry.has<CTransformComponent>(entity))
			{
				if (ImGui::Selectable("Transform Component", false))
				{
					selected = ESelected::Transform;
					registry.emplace<CTransformComponent>(entity);
					PSavingSystem::GetHandle()->RegisterDirtyObject(m_entity);
				}
			}
			if (!registry.has<CMeshComponent>(entity))
			{
				if (ImGui::Selectable("Mesh Component", false))
				{
					selected = ESelected::Mesh;
					registry.emplace<CMeshComponent>(entity);
					PSavingSystem::GetHandle()->RegisterDirtyObject(m_entity);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::EndChild();
	}

	ImGui::SameLine();

	if (ImGui::BeginChild("Details", ImVec2(ImGui::GetWindowWidth() - 325, 0), true))
	{
	}

	ImGui::End();
}

void Protostar::PEntityEditor::CloseEditor()
{
	m_entity = nullptr;
}
