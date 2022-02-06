#ifndef LOG_H
#define LOG_H

#include "Guid.h"
#include "GlobalMacros.h"
#include "Util.h"

#include <string>
#include <unordered_map>
#include <mutex>

#define LOGFORMAT(LOG) Protostar::Printf("%s[%i] | %s", __FUNCTION__, __LINE__, LOG)

#define PE_LOG(LOG) PLog::Log(LOGFORMAT(LOG))
#define PE_LOG_F(LOG, ...) PLog::Log(Protostar::Printf(LOGFORMAT(LOG), __VA_ARGS__))
#if _DEBUG
#define PE_DEBUG_LOG(LOG) PLog::LogDebugString(LOGFORMAT(LOG))
#else
#define PE_DEBUG_LOG(LOG) 
#endif

#if _DEBUG
#define PE_DEBUG_LOG_F(LOG, ...) PLog::LogDebugString(Protostar::Printf(LOGFORMAT(LOG), __VA_ARGS__))
#else
#define PE_DEBUG_LOG_F(LOG, ...) 
#endif

namespace Protostar
{
	struct PConsoleInfo;

	struct PLogInfo
	{
		std::string& String;
		PGuid* HandleGuids;
		size_t HandleAmount;
	};

	struct PColor
	{
		float Color[4];
	};

	class PLog
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
		static void RegisterHandle(const PGuid& _handleGuid, const PConsoleInfo& _outputInfo);

		static void RegisterBuffer(const PGuid& _bufGuid, std::vector<std::pair<PColor, std::string>>* buf);

		static void RemoveBuffer(const PGuid& _bufGuid);

		static bool IsBufRegistered(const PGuid& _bufGuid);

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
		static void Log(const PGuid& _handleGuid, const std::string& _logString);

		/**
		* Clears the screen of the guid with the handle
		* @param _handleGuid The Guid of the handle to clear
		*/
		static void ClearLog(const PGuid& _handleGuid);

		/**
		* Log to all registered handles with the guids 
		* @param _handleGuids The guids of the handles to log to
		* @param _handleGuidAmount The amount of guids the handles to log to
		* @param _logString The string to log
		*/
		static void Log(PGuid* _handleGuids, const size_t _handleGuidAmount, const std::string& _logString);

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
		static void ClearLog(PGuid* _handleGuids, const size_t _handleGuidAmount);
	private:
		PLog() = default;

		/**
		* Actually does the log to the handle
		* @param _handle The handle to log to
		* @param _logString The string to log
		*/
		static void LogToHandle(PConsoleInfo& _info, const std::string& _logString);

		/**
		* Actually clears the handle
		* @param _handle The handle to clear
		*/
		static void ClearLogHandle(PConsoleInfo& _info);

		static PLog* s_logHandle;

		std::unordered_map<PGuid, PConsoleInfo> m_logInfos;
		std::unordered_map<PGuid, std::vector<std::pair<PColor, std::string>>*> m_logBuffer;

		std::vector<std::pair<PColor, std::string>> m_buffer;

		std::mutex m_logMutex;
	};
}
#endif //LOG_H