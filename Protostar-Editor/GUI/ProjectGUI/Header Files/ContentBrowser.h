#pragma once
#include "Filesystem/FileTreeObject.h"
#include "ShaderLibrary.h"
#include "Guid.h"
import <filesystem>;
import <unordered_map>;

namespace Protostar
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
		void DisplayRecursiveShaderTrees(const STreeObject<SShaderComplement>& _subTree);
		void ManageModelPaths();
		void ManageScenePaths();
		void ShowEntities();
		void RenderComponents();

		void CheckPaths();
		void InitSceneTreeNode();

		CProject* m_project;
		class CMeshManager* m_meshManager;
		bool m_bDelKeyPressedThisFrame;
		bool m_bRefresh;
		std::unordered_map<class CScene*, bool> m_scenes;
		std::unordered_map<class CBaseEntity*, bool> m_entities;
	};
}