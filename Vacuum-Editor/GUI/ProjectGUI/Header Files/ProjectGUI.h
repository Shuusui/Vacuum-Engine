#pragma once
#include <string>

namespace Vacuum
{
	class CProjectGUI
	{
	public:
		static void OnCreate();
		static void OnDestroy();
		static CProjectGUI* GetProjectGUIHandle()
		{
			return s_projectGUI;
		}
		~CProjectGUI();
		void OnRender();
		void RenderViewport();
		void RenderCreateSceneWindow();
		void RenderCreateEntityWindow();
		class CEntityEditor* GetEntityEditorHandle()
		{
			return m_entityEditor;
		}

		void SetShowCreateSceneWindow(const bool& bCreateSceneWindowState)
		{
			m_bShowCreateSceneWindow = bCreateSceneWindowState;
		}

		void SetShowCreateEntityWindow(const bool& bCreateEntityWindowState)
		{
			m_bShowCreateEntityWindow = bCreateEntityWindowState;
		}

		void ToggleContentBrowser();

	private:
		CProjectGUI();

		static CProjectGUI* s_projectGUI;

		class CProject* m_currentProject;
		class CSavingSystem* m_savingSystem;
		class CContentBrowser* m_contentBrowser;
		class CEntityEditor* m_entityEditor;
		size_t m_setProjectFuncIndex;
		bool m_bShowCreateSceneWindow;
		bool m_bShowCreateEntityWindow;
		bool m_bShowEntityEditorWindow;
		char m_newSceneName[256];
		char m_newEntityName[256];
	};
}