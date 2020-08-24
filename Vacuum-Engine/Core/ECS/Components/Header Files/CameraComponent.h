#pragma once
#include "BaseComponent.h"

namespace Vacuum
{
	class CCameraComponent : public CBaseComponent
	{
	public:
		CCameraComponent()
			:CBaseComponent("Camera Component")
		{

		}
		virtual void OnSave() override
		{

		}
	};
}