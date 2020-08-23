#pragma once


namespace Vacuum
{
	class CBaseEntity;

	class CEntityEditor
	{
	public: 
		static CEntityEditor* OnCreate();
		void OpenEditor(CBaseEntity* _entity);
		void OnRender();
		void CloseEditor();
	private:
		static CEntityEditor* s_entityEditor;

		CBaseEntity* m_entity;
	};
}