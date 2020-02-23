#include "../HeaderFiles/Log.h"

namespace Vacuum
{
	namespace Core
	{
		CLog* CLog::s_logHandle = nullptr;

		bool CLog::Init(std::string& errorMsg)
		{
			if (s_logHandle)
			{
				errorMsg = "Log handle is already initialized";
				return false;
			}
			s_logHandle = new CLog();
		}

		bool CLog::RegisterHandle(const GUID& handleGuid, const HANDLE& outputHandle)
		{

		}
	}
}