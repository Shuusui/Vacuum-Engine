#pragma once

#pragma region Includes

#pragma region Internal Includes
#include "Guid.h"
#include "GlobalMacros.h"
#include "Util.h"
#pragma endregion Internal Includes

#pragma region External Includes
import <string>;
import <unordered_map>;
import <mutex>;
#pragma endregion External Includes

#pragma endregion Includes

#define LOGFORMAT(LOG) Protostar::Printf("%s[%i] | %s", __FUNCTION__, __LINE__, LOG)

#define PE_LOG(LOG) CLog::Log(LOGFORMAT(LOG))
#define PE_LOG_F(LOG, ...) CLog::Log(Protostar::Printf(LOGFORMAT(LOG), __VA_ARGS__))
#define PE_DEBUG_LOG(LOG) CLog::LogDebugString(LOGFORMAT(LOG))
#define PE_DEBUG_LOG_F(LOG, ...) CLog::LogDebugString(Protostar::Printf(LOGFORMAT(LOG), __VA_ARGS__))

namespace Protostar
{
	struct SConsoleInfo;

	struct SLogInfo
	{
		std::string& String;
		SGuid* HandleGuids;
		size_t HandleAmount;
	};

	struct SColor
	{
		float Color[4];
	};

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
		* @param _outputHandle The actual output handle where the log system will log to
		*/
		static void RegisterHandle(const SGuid& _handleGuid, const SConsoleInfo& _outputInfo);

		static void RegisterBuffer(const SGuid& _bufGuid, std::vector<std::pair<SColor, std::string>>* buf);

		static void RemoveBuffer(const SGuid& _bufGuid);

		static bool IsBufRegistered(const SGuid& _bufGuid);

		/**
		* Logs to all registered console handles
		* @param _logString The string to log
		*/
		static void Log(const std::string& _logString);

		/**
		* Clears the screen of all log handles
		*/
		static void ClearAllLogs();

		/**
		* Logs only to the console handles with the right guid
		* @param _handleGuid The guid of the handle to log to
		* @param _logString The string to log
		*/
		static void Log(const SGuid& _handleGuid, const std::string& _logString);

		/**
		* Clears the screen of the guid with the handle
		* @param _handleGuid The Guid of the handle to clear
		*/
		static void ClearLog(const SGuid& _handleGuid);

		/**
		* Log to all registered handles with the guids 
		* @param _handleGuids The guids of the handles to log to
		* @param _handleGuidAmount The amount of guids the handles to log to
		* @param _logString The string to log
		*/
		static void Log(SGuid* _handleGuids, const size_t _handleGuidAmount, const std::string& _logString);

#if defined(_DEBUG)
		/**
		* Just logs a debug string to the output window of VS
		* @param _logString The string to log
		*/
		static void LogDebugString(const std::string& _logString);
#endif

		/**
		* Clears the handles with the guids
		* @param _handleGuids The guids of the handles to clear
		* @param _handleGuidAmount The amount of guids the handles to clear
		*/
		static void ClearLog(SGuid* _handleGuids, const size_t _handleGuidAmount);
	private:
		CLog() = default;

		/**
		* Actually does the log to the handle
		* @param _handle The handle to log to
		* @param _logString The string to log
		*/
		static void LogToHandle(SConsoleInfo& _info, const std::string& _logString);

		/**
		* Actually clears the handle
		* @param _handle The handle to clear
		*/
		static void ClearLogHandle(SConsoleInfo& _info);

		static CLog* s_logHandle;

		std::unordered_map<SGuid, SConsoleInfo> m_logInfos;
		std::unordered_map<SGuid, std::vector<std::pair<SColor, std::string>>*> m_logBuffer;

		std::vector<std::pair<SColor, std::string>> m_buffer;

		std::mutex m_logMutex;
	};
}