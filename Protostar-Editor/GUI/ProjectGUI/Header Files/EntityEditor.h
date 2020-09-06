#pragma once


namespace Protostar
{
	class CBaseEntity;

	class CEntityEditor
	{
	public: 
		static CEntityEditor* OnCreate();
		CEntityEditor()
			:m_entity(nullptr)
			,m_bEntityEditorOpen(false)
		{

		}
		void OpenEditor(CBaseEntity* _entity);
		void OnRender();
		void CloseEditor();
	private:
		static CEntityEditor* s_entityEditor;

		CBaseEntity* m_entity;
		bool m_bEntityEditorOpen;
	};
}