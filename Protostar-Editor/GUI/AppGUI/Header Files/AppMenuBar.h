#pragma once
#include <unordered_map>

namespace Protostar
{
	class PAppMenuBar 
	{
	public:
		PAppMenuBar();
		~PAppMenuBar();
		void OnRender();
		void ShowSaveWindow();
	private:
		class PLogWindow* m_logWindow;
		class PEditorFPS* m_editorFPS;

		bool m_bShowSaveWindow;
		std::unordered_map<class PBaseObject*, bool> m_dirtyObjects;

	};
}