#pragma once
#include "BaseComponent.h"

namespace Vacuum
{
	class CMeshComponent : public CBaseComponent
	{
	public:
		CMeshComponent()
			:CBaseComponent("Mesh Component")
		{

		}

		virtual void OnSave() override
		{

		}
	};
}