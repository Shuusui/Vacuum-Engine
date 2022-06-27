#pragma once

#include "BaseComponent.h"
#include "MathExt.h"

namespace Protostar
{
	class CTransformComponent : public CBaseComponent
	{
	public:
		CTransformComponent()
			:CBaseComponent("Transform Component")
		{
		}

		virtual void OnSave() override
		{
		}
	};
}

