#pragma once

#pragma region Includes

#pragma region Internal Includes
#include "Guid.h"
#pragma endregion Internal Includes

#pragma region External Includes
#include <string>
#include <unordered_map>
#pragma endregion External Includes

#pragma endregion Includes

namespace Vacuum
{
	namespace Core
	{
		class CLog
		{
		public: 
			/*
			* Needs to get called to initialize the log in first place
			* @param errorMsg If the calll fails this string will get filled
			* @return if it's successful or not
			*/
			static bool Init(std::string& _errorMsg);

			/*
			* registeres a new handle with a guid which will get stored to make broadcasting over different handles possible
			* @param handleGuid the guid of the handle to call it
			* @param outputHandle the actual outputhandle where the log system will log to
			* @return if the handle guid already exists this will return false otherwise true
			*/
			static void RegisterHandle(const SGuid& _handleGuid, const HANDLE& _outputHandle);

			static void Log(const std::wstring& _logString);
		private:
			CLog() = default;
			static CLog* s_logHandle;

			std::unordered_map<SGuid, HANDLE> m_logHandles;
		};
	}
}