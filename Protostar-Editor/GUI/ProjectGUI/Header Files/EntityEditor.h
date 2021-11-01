#pragma once


namespace Protostar
{
	class PBaseEntity;

	class PEntityEditor
	{
	public: 
		static PEntityEditor* OnCreate();
		PEntityEditor()
			:m_entity(nullptr)
			,m_bEntityEditorOpen(false)
		{

		}
		void OpenEditor(PBaseEntity* _entity);
		void OnRender();
		void CloseEditor();
	private:
		static PEntityEditor* s_entityEditor;

		PBaseEntity* m_entity;
		bool m_bEntityEditorOpen;
	};
}