#pragma once

#include "..\..\Header Files\BaseObject.h"
#include <typeinfo>

namespace Vacuum
{
	class CBaseComponent : public CBaseObject
	{
	public: 
		CBaseComponent();
		virtual ~CBaseComponent();
	};
}
