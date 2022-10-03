#pragma once

#include "BaseComponent.h"
#include "Guid.h"

namespace Protostar
{
	class CMeshComponent : public CBaseComponent
	{
	public:
		CMeshComponent()
			:CBaseComponent("Mesh Component")
			, m_guid(Guid::NewGuid())
			, m_modelGuid(Guid())
		{

		}

		virtual void OnSave() override
		{
		}

		Guid GetGuid() const
		{
			return m_guid;
		}

		Guid GetModelGuid() const
		{
			return m_modelGuid;
		}

		void SetGuid(const Guid& _guid)
		{
			m_guid = _guid;
		}

		void SetModelGuid(const Guid& _guid)
		{
			m_modelGuid = _guid;
		}

	private:
		Guid m_guid;
		Guid m_modelGuid;
	};
}

