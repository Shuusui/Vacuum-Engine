#pragma once

#include "Guid.h"
#include "Util.h"
#include "MainCore.h"

#include <string>
#include <unordered_map>
#include <mutex>

#define LOGFORMAT(LOG) Protostar::Core::Printf("%s[%i] | %s", __FUNCTION__, __LINE__, LOG)

#define PE_LOG(LOG) Protostar::Core::Logger::Log(LOGFORMAT(LOG))
#define PE_LOG_F(LOG, ...) Protostar::Core::Logger::Log(Protostar::Core::Printf(LOGFORMAT(LOG), __VA_ARGS__))
#if _DEBUG
#define PE_DEBUG_LOG(LOG) Protostar::Core::Logger::LogDebugString(LOGFORMAT(LOG))
#else
#define PE_DEBUG_LOG(LOG) 
#endif

#if _DEBUG
#define PE_DEBUG_LOG_F(LOG, ...) Protostar::Core::Logger::LogDebugString(Protostar::Core::Printf(LOGFORMAT(LOG), __VA_ARGS__))
#else
#define PE_DEBUG_LOG_F(LOG, ...) 
#endif

namespace Protostar::Core
{

	struct MAINCORE_API Color
	{
		float Color[4];
	};
	struct MAINCORE_API ConsoleInfo
	{
		COORD ConsolePos;
		HANDLE ConsoleHandle;
	};

	struct ConsoleHandles
	{
		ConsoleInfo InputConInfo;
		ConsoleInfo OutputConInfo;
		ConsoleInfo ErrorConInfo;
		Guid HandlesGuid;
	};

	class MAINCORE_API Logger
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
		static void RegisterHandle(const Guid& _handleGuid, const ConsoleInfo& _outputInfo);

		static void RegisterBuffer(const Guid& _bufGuid, std::vector<std::pair<Color, std::string>>* buf);

		static void RemoveBuffer(const Guid& _bufGuid);

		static bool IsBufRegistered(const Guid& _bufGuid);

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
		static void Log(const Guid& _handleGuid, const std::string& _logString);

		/**
		* Clears the screen of the guid with the handle
		* @param _handleGuid The Guid of the handle to clear
		*/
		static void ClearLog(const Guid& _handleGuid);

		/**
		* Log to all registered handles with the guids
		* @param _handleGuids The guids of the handles to log to
		* @param _handleGuidAmount The amount of guids the handles to log to
		* @param _logString The string to log
		*/
		static void Log(Guid* _handleGuids, const size_t _handleGuidAmount, const std::string& _logString);

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
		static void ClearLog(Guid* _handleGuids, const size_t _handleGuidAmount);
	private:
		Logger() = default;

		struct LogInfo
		{
			std::string& String;
			Guid* HandleGuids;
			size_t HandleAmount;
		};


		/**
		* Actually does the log to the handle
		* @param _handle The handle to log to
		* @param _logString The string to log
		*/
		static void LogToHandle(ConsoleInfo& _info, const std::string& _logString);

		/**
		* Actually clears the handle
		* @param _handle The handle to clear
		*/
		static void ClearLogHandle(ConsoleInfo& _info);

		static Logger* s_loggerHandle;

		std::unordered_map<Guid, ConsoleInfo> m_logInfos;
		std::unordered_map<Guid, std::vector<std::pair<Color, std::string>>*> m_logBuffer;

		std::vector<std::pair<Color, std::string>> m_buffer;

		std::mutex m_logMutex;
	};
}

