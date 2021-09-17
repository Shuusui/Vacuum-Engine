#pragma once
import <unordered_map>;

namespace Protostar
{
	class CAppMenuBar 
	{
	public:
		CAppMenuBar();
		~CAppMenuBar();
		void OnRender();
		void ShowSaveWindow();
	private:
		class CLogWindow* m_logWindow;
		class CEditorFPS* m_editorFPS;

		bool m_bShowSaveWindow;
		std::unordered_map<class CBaseObject*, bool> m_dirtyObjects;

	};
}