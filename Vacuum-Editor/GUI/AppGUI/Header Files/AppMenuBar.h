#pragma once

namespace Vacuum
{
	class CAppMenuBar 
	{
	public:
		CAppMenuBar();
		~CAppMenuBar();
		void OnRender();
	private:
		class CLogWindow* m_logWindow;
		class CContentBrowser* m_contentBrowser;
		class CEditorFPS* m_editorFPS;
	};
}