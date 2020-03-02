#include "../HeaderFiles/Log.h"

namespace Vacuum
{
	namespace Core
	{
		CLog* CLog::s_logHandle = nullptr;

		bool CLog::Init(std::string& _errorMsg)
		{
			if (s_logHandle)
			{
				_errorMsg = "Log handle is already initialized";
				return false;
			}
			s_logHandle = new CLog();
			return true;
		}

		void CLog::RegisterHandle(const SGuid& _handleGuid, const HANDLE& _outputHandle)
		{
			s_logHandle->m_logHandles.insert(std::make_pair(_handleGuid, _outputHandle));
		}
	}
}