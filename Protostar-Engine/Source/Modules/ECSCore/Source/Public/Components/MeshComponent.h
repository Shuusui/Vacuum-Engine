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
			, m_guid(PGuid::NewGuid())
			, m_modelGuid(PGuid())
		{

		}

		virtual void OnSave() override
		{
		}

		PGuid GetGuid() const
		{
			return m_guid;
		}

		PGuid GetModelGuid() const
		{
			return m_modelGuid;
		}

		void SetGuid(const PGuid& _guid)
		{
			m_guid = _guid;
		}

		void SetModelGuid(const PGuid& _guid)
		{
			m_modelGuid = _guid;
		}

	private:
		PGuid m_guid;
		PGuid m_modelGuid;
	};
}

