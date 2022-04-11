#include "Guid.h"

#include "GlobalMacros.h"

#ifdef _WIN
#include "Windows.h"
namespace Protostar
{
	PGuid PGuid::NewGuid()
	{
		PGuid returnGuid;
		HRESULT result = CoCreateGuid((GUID*)&returnGuid);
		if (result != S_OK)
		{
			return PGuid();
		}
		return returnGuid;
	}
}
#endif