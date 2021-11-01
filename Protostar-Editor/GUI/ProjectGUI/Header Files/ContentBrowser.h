#pragma once
#include <filesystem>
#include <unordered_map>
#include "Filesystem/FileTreeObject.h"
#include "ShaderLibrary.h"
#include "Guid.h"

namespace Protostar
{
	class PProject;

	class PContentBrowser
	{
	public:
		PContentBrowser(PProject* _project);
		void OnRender();

	private:
		void DisplayContextMenu();
		void ManageShaderPaths();
		void DisplayRecursiveShaderTrees(const PTreeObject<SShaderComplement>& _subTree);
		void ManageModelPaths();
		void ManageScenePaths();
		void ShowEntities();
		void RenderComponents();

		void CheckPaths();
		void InitSceneTreeNode();

		PProject* m_project;
		class PMeshManager* m_meshManager;
		bool m_bDelKeyPressedThisFrame;
		bool m_bRefresh;
		std::unordered_map<class PScene*, bool> m_scenes;
		std::unordered_map<class PBaseEntity*, bool> m_entities;
	};
}