#include "Guid.h"

#include "GlobalMacros.h"

#ifdef _WIN
#include "Windows.h"

namespace Protostar::Core
{
	Guid Guid::NewGuid()
	{
		Guid returnGuid;
		HRESULT result = CoCreateGuid((GUID*)&returnGuid);
		if (result != S_OK)
		{
			return Guid();
		}
		return returnGuid;
	}
}
#endif