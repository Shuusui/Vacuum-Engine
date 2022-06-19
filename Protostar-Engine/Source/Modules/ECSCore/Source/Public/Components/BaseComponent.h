#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include "ECS\Header Files\BaseObject.h"

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
#endif //BASECOMPONENT_H