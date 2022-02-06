#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

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

#endif //TRANSFORMCOMPONENT_H