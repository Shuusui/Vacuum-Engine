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

#define VE_LOG(LOG, ...) CLog::Log()

namespace Vacuum
{
	namespace Core
	{
		class CLog
		{
		public:
			/**
			* Needs to get called to initialize the log in first place
			* @param _errorMsg If the call fails this string will get filled
			* @return if it's successful or not
			*/
			static bool Init(std::string& _errorMsg);

			/**
			* registers a new handle with a guid which will get stored to make broadcasting over different handles possible. This will overwrite a handle with the same guid if it already exists.
			* @param _handleGuid The guid of the handle to call it
			* @param _outputHandle The actual outputhandle where the log system will log to
			*/
			static void RegisterHandle(const SGuid& _handleGuid, const HANDLE& _outputHandle);

			/**
			* Logs to all registered console handles
			* @param _logString The string to log
			*/
			static void Log(const std::wstring& _logString);

			/**
			* Logs only to the console handles with the right guid
			* @param _handleGuid The guid of the handle to log to
			* @param _logString The string to log
			*/
			static void Log(const SGuid& _handleGuid, const std::wstring& _logString);

			/**
			* Log to all registered handles with the guids 
			* @param _handleGuids The guids of the handles to log to
			* @param _handleGuidAmount The amount of guids the handles to log to
			* @param _logString The string to log
			*/
			static void Log(SGuid* _handleGuids, const size_t& _handleGuidAmount, const std::wstring& _logString);
		private:
			CLog() = default;

			/**
			* Actually does the log to the handle
			* @param _handle The handle to log to
			* @param _logString The string to log
			*/
			static void LogToHandle(const HANDLE& _handle, const std::wstring& _logString);

			static CLog* s_logHandle;

			std::unordered_map<SGuid, HANDLE> m_logHandles;
		};
	}
}