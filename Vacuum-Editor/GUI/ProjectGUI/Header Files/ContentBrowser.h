#pragma once
#include <filesystem>
#include <unordered_map>
#include "Guid.h"

namespace Vacuum
{
	class CProject;

	class CContentBrowser
	{
	public:
		CContentBrowser(CProject* _project);
		void OnRender();

	private:
		void DisplayContextMenu();
		void ManageShaderPaths();
		void ManageModelPaths();
		void ManageScenePaths();
		void ShowEntities();
		void RenderComponents();

		void CheckPaths();
		void InitSceneTreeNode();

		CProject* m_project;
		bool m_bDelKeyPressedThisFrame;
		bool m_bRefresh;
		std::unordered_map<class CScene*, bool> m_scenes;
		std::unordered_map<class CBaseEntity*, bool> m_entities;
	};
}