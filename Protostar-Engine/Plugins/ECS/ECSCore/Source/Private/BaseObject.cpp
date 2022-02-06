#include "..\Header Files\BaseObject.h"
#include "GlobalDefinitions.h"
#include <atomic>

#if defined(_DEBUG)
static std::atomic_int64_t s_ObjectCounter(0);
#endif

Protostar::PBaseObject::PBaseObject(const std::string& _objectName)
	:m_guid(PGuid::NewGuid())
	,m_objectName(_objectName)
	,m_className(typeid(this).raw_name())

{
#if defined(_DEBUG)
	s_ObjectCounter.fetch_add(1);
#endif
}

Protostar::PBaseObject::~PBaseObject()
{
#if defined(_DEBUG)
	s_ObjectCounter.fetch_sub(1);
#endif
}
