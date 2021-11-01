#pragma once
#include "ECS\Header Files\BaseObject.h"
#include "Json.h"

namespace Protostar
{
	class CBaseComponent : public PBaseObject
	{
	public: 
		CBaseComponent(const std::string& _name, const PJson& _json)
			:PBaseObject(_name)
			,m_jsonObject(_json)
		{
		}

		PJson GetJsonObject() const
		{
			return m_jsonObject;
		}

		virtual void OnSave() = 0;
	protected:
		PJson m_jsonObject;
	};
}