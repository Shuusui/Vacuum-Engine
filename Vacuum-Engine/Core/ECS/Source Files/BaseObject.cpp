#include "..\Header Files\BaseObject.h"
#include "GlobalDefs.h"
#include <atomic>

#if defined(_DEBUG)
static std::atomic_int64_t s_ObjectCounter = 0;
#endif

Vacuum::CBaseObject::CBaseObject(const std::string& _objectName)
	:m_guid(SGuid::NewGuid())
	,m_objectName(_objectName)
	,m_className(typeid(this).raw_name())

{
#if defined(_DEBUG)
	s_ObjectCounter.fetch_add(1);
#endif
}

Vacuum::CBaseObject::~CBaseObject()
{
#if defined(_DEBUG)
	s_ObjectCounter.fetch_sub(1);
#endif
}
