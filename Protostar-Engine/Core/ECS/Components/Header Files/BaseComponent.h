#pragma once
#include "ECS\Header Files\BaseObject.h"
#include "Json.h"

namespace Protostar
{
	class CBaseComponent : public CBaseObject
	{
	public: 
		CBaseComponent(const std::string& _name, const Json& _json)
			:CBaseObject(_name)
			,m_jsonObject(_json)
		{
		}

		Json GetJsonObject() const
		{
			return m_jsonObject;
		}

		virtual void OnSave() = 0;
	protected:
		Json m_jsonObject;
	};
}