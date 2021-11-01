#pragma once
#include <string>

namespace Protostar
{
	class PProjectGUI
	{
	public:
		static void OnCreate();
		static void OnDestroy();
		static PProjectGUI* GetProjectGUIHandle()
		{
			return s_projectGUI;
		}
		~PProjectGUI();
		void OnRender();
		void RenderViewport();
		void RenderCreateSceneWindow();
		void RenderCreateEntityWindow();
		class PEntityEditor* GetEntityEditorHandle()
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
		PProjectGUI();

		static PProjectGUI* s_projectGUI;

		class PProject* m_currentProject;
		class PSavingSystem* m_savingSystem;
		class PContentBrowser* m_contentBrowser;
		class PEntityEditor* m_entityEditor;
		size_t m_setProjectFuncIndex;
		bool m_bShowCreateSceneWindow;
		bool m_bShowCreateEntityWindow;
		bool m_bShowEntityEditorWindow;
		char m_newSceneName[256];
		char m_newEntityName[256];
	};
}