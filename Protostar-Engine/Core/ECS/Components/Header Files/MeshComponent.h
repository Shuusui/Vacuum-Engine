#pragma once
#include "BaseComponent.h"
#include "Guid.h"

#define JSONMESHCOMPGUID "guid"
#define JSONMESHCOMPMODELGUID "model_guid"

namespace Protostar
{
	class CMeshComponent : public CBaseComponent
	{
	public:
		CMeshComponent()
			:CBaseComponent("Mesh Component",PJson())
			, m_guid(PGuid::NewGuid())
			, m_modelGuid(PGuid())
		{

		}

		CMeshComponent(const PJson& _json)
			:CBaseComponent("Mesh Component", _json)
		{
			m_jsonObject = _json;
			m_guid = m_jsonObject[JSONMESHCOMPGUID].get<std::string>();
			m_modelGuid = m_jsonObject[JSONMESHCOMPMODELGUID].get<std::string>();
		}

		virtual void OnSave() override
		{
			m_jsonObject =
			{
				{JSONMESHCOMPGUID, m_guid.ToString()},
				{JSONMESHCOMPMODELGUID, m_modelGuid.ToString()}
			};
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

#undef JSONMESHCOMPGUID
#undef JSONMESHCOMPMODELGUID