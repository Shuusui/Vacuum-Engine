#include "..\Header Files\EntityEditor.h"
#include "imgui.h"
#include <string>
#include "ECS\Entities\Header Files\BaseEntity.h"
#include "ECS\Header Files\ECSManager.h"
#include "ECS\Components\Header Files\TransformComponent.h"
#include "SavingSystem.h"

Vacuum::CEntityEditor* Vacuum::CEntityEditor::s_entityEditor = nullptr;

enum class ESelected
{
	None,
	Transform
};

Vacuum::CEntityEditor* Vacuum::CEntityEditor::OnCreate()
{
	if (s_entityEditor)
	{
		return s_entityEditor;
	}

	return s_entityEditor = new CEntityEditor();
}

void Vacuum::CEntityEditor::OpenEditor(CBaseEntity* _entity)
{
	m_entity = _entity;
}

void Vacuum::CEntityEditor::OnRender()
{
	if (!m_entity)
	{
		return;
	}

	ImVec2 windowPos = ImGui::GetWindowPos();
	ImGui::SetWindowPos(ImVec2(windowPos.x * 0.5f, windowPos.y * 0.5f), ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
	if(!ImGui::Begin("Entity Editor", (bool*)m_entity))
	{
		ImGui::End();
	}

	entt::registry& registry = CECSManager::GetRegistry();
	entt::entity& entity = m_entity->GetEntity();

	static ESelected selected = ESelected::None;

	if (ImGui::BeginChild("Overview", ImVec2(300, 0), true))
	{
		ImGui::Text("Overview:");
		ImGui::Text("Name:" );
		ImGui::SameLine();
		ImGui::Text(m_entity->GetObjectName().c_str());
		if (registry.has<CTransformComponent>(entity))
		{
			if (ImGui::Selectable("Transform Component", selected == ESelected::Transform))
			{
				selected = ESelected::Transform;
			}
		}
		else
		{
			if (ImGui::Button("Add transform component"))
			{
				selected = ESelected::Transform;
				registry.emplace<CTransformComponent>(entity);
				CSavingSystem::GetHandle()->RegisterDirtyObject(m_entity);
			}
		}
		ImGui::EndChild();
	}

	ImGui::SameLine();

	if (ImGui::BeginChild("Details", ImVec2(ImGui::GetWindowWidth() - 325, 0), true))
	{
		ImGui::Text("Details: ");
		if (selected == ESelected::Transform)
		{
			CTransformComponent& transform = registry.get<CTransformComponent>(entity);

			bool bShouldSave = false;

			float pos[3] = {transform.GetPos().x, transform.GetPos().y, transform.GetPos().z};
			if (ImGui::InputFloat3("Position", pos, 4))
			{
				transform.SetPos({ pos[0], pos[1], pos[2] });
				bShouldSave = true;
			}

			float rot[3] = { transform.GetRot().x, transform.GetRot().y, transform.GetRot().z };
			if (ImGui::InputFloat3("Rotation", rot, 4))
			{
				transform.SetRot({rot[0], rot[1], rot[2]});
				bShouldSave = true;
			}

			float scale[3] = { transform.GetScale().x, transform.GetScale().y, transform.GetScale().z };
			if (ImGui::InputFloat3("Scale", scale, 4))
			{
				transform.SetScale({ scale[0], scale[1], scale[2] });
				bShouldSave = true;
			}

			if (bShouldSave)
			{
				CSavingSystem::GetHandle()->RegisterDirtyObject(m_entity);
			}
		}
		ImGui::EndChild();
	}

	ImGui::End();
}

void Vacuum::CEntityEditor::CloseEditor()
{
	m_entity = nullptr;
}
