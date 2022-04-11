#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "BaseComponent.h"

namespace Protostar
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
#endif //CAMERACOMPONENT_H