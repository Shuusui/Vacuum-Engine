#pragma once
#include "ECS\Header Files\BaseObject.h"
#include "Json.h"

namespace Vacuum
{
	class CBaseComponent : public CBaseObject
	{
	public: 
		CBaseComponent(const std::string& _name)
			:CBaseObject(_name)
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