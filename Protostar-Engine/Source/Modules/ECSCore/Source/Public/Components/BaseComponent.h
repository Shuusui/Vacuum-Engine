#pragma once

#include "BaseObject.h"

namespace Protostar
{
	class CBaseComponent : public PBaseObject
	{
	public: 
		CBaseComponent(const std::string& _name)
			:PBaseObject(_name)
		{
		}

		virtual void OnSave() = 0;
	protected:
	};
}
