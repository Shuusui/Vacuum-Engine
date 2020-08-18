#pragma once


namespace Vacuum
{
	class CProjectGUI
	{
	public:
		CProjectGUI();
		~CProjectGUI();
		void OnRender();
	private:
		class CProject* m_currentProject;
		size_t m_setProjectFuncIndex;
	};
}