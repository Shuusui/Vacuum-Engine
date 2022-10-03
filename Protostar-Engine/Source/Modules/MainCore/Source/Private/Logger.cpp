#include "Logger.h"

namespace Protostar::Core
{
	Logger* Logger::s_loggerHandle = nullptr;

	bool Logger::Init(std::string& _errorMsg)
	{
		if (s_loggerHandle)
		{
			_errorMsg = "Log handle is already initialized";
			return false;
		}
		s_loggerHandle = new Logger();
		return true;
	}

	void Logger::RegisterHandle(const Guid& _handleGuid, const ConsoleInfo& _outputInfo)
	{
		s_loggerHandle->m_logMutex.lock();
		s_loggerHandle->m_logInfos.insert(std::make_pair(_handleGuid, _outputInfo));
		s_loggerHandle->m_logMutex.unlock();
	}

	void Logger::RegisterBuffer(const Guid& _bufGuid, std::vector<std::pair<Color, std::string>>* buf)
	{
		*buf = s_loggerHandle->m_buffer;
		s_loggerHandle->m_logBuffer.insert(std::make_pair(_bufGuid, buf));
	}

	void Logger::RemoveBuffer(const Guid& _bufGuid)
	{
		s_loggerHandle->m_logBuffer.erase(_bufGuid);
	}

	bool Logger::IsBufRegistered(const Guid& _bufGuid)
	{
		return s_loggerHandle->m_logBuffer.find(_bufGuid) == s_loggerHandle->m_logBuffer.end();
	}

	void Logger::Log(const std::string& _logString)
	{
		s_loggerHandle->m_logMutex.lock();
		s_loggerHandle->m_buffer.push_back(std::make_pair(Color{ 1.0f, 0.4f, 0.8f, 1.0f }, _logString));
		for (std::pair<const Guid, ConsoleInfo>& handlePair : s_loggerHandle->m_logInfos)
		{
			LogToHandle(handlePair.second, _logString);
		}

		for (const std::pair<Guid, std::vector<std::pair<Color, std::string>>*>& bufPair : s_loggerHandle->m_logBuffer)
		{
			Color color = { 1.0f, 0.4f, 0.8f, 1.0f };
			bufPair.second->push_back(std::make_pair(color, _logString));
		}

		OutputDebugStringA(_logString.c_str());
		OutputDebugStringA("\n");

		s_loggerHandle->m_logMutex.unlock();
	}

	void Logger::ClearAllLogs()
	{
		s_loggerHandle->m_logMutex.lock();
		for (std::pair<const Guid, ConsoleInfo>& handlePair : s_loggerHandle->m_logInfos)
		{
			ClearLogHandle(handlePair.second);
		}
		s_loggerHandle->m_logMutex.unlock();
	}

	void Logger::Log(const Guid& _handleGuid, const std::string& _logString)
	{
		s_loggerHandle->m_logMutex.lock();
		LogToHandle(s_loggerHandle->m_logInfos.at(_handleGuid), _logString);
		s_loggerHandle->m_logMutex.unlock();
	}

	void Logger::ClearLog(const Guid& _handleGuid)
	{
		s_loggerHandle->m_logMutex.lock();
		ClearLogHandle(s_loggerHandle->m_logInfos.at(_handleGuid));
		s_loggerHandle->m_logMutex.unlock();
	}

	void Logger::Log(Guid* _handleGuids, const size_t _handleGuidAmount, const std::string& _logString)
	{
		s_loggerHandle->m_logMutex.lock();
		for (s32 i = 0; i < _handleGuidAmount; ++i)
		{
			LogToHandle(s_loggerHandle->m_logInfos.at(_handleGuids[i]), _logString);
		}
		s_loggerHandle->m_logMutex.unlock();
	}

	void Logger::LogDebugString(const std::string& _logString)
	{
		OutputDebugStringA(_logString.c_str());
		OutputDebugStringA("\n");
	}

	void Logger::ClearLog(Guid* _handleGuids, const size_t _handleGuidAmount)
	{
		s_loggerHandle->m_logMutex.lock();
		for (s32 i = 0; i < _handleGuidAmount; ++i)
		{
			ClearLogHandle(s_loggerHandle->m_logInfos.at(_handleGuids[i]));
		}
		s_loggerHandle->m_logMutex.unlock();
	}

	void Logger::LogToHandle(ConsoleInfo& _info, const std::string& _logString)
	{
		if (!_info.ConsoleHandle)
		{
			return;
		}
		CONSOLE_SCREEN_BUFFER_INFO screenBuf;
		GetConsoleScreenBufferInfo(_info.ConsoleHandle, &screenBuf);

		CONSOLE_CURSOR_INFO cursor;
		cursor.dwSize = 1;
		cursor.bVisible = FALSE;
		SetConsoleCursorInfo(_info.ConsoleHandle, &cursor);

		DWORD garbage = 0;

		for (s32 i = 0; i < _logString.size(); ++i)
		{
			FillConsoleOutputCharacterA(_info.ConsoleHandle, _logString[i], 1, _info.ConsolePos, &garbage);
			_info.ConsolePos.X++;
		}
		_info.ConsolePos.Y++;
		_info.ConsolePos.X = 0;
		SetConsoleCursorPosition(_info.ConsoleHandle, _info.ConsolePos);
		OutputDebugStringA(_logString.c_str());
		OutputDebugStringA("\n");
	}

	void Logger::ClearLogHandle(ConsoleInfo& _info)
	{
		if (!_info.ConsoleHandle)
		{
			return;
		}
		CONSOLE_SCREEN_BUFFER_INFO screenBuf;
		GetConsoleScreenBufferInfo(_info.ConsoleHandle, &screenBuf);
		DWORD dwConSize = screenBuf.dwSize.X * screenBuf.dwSize.Y;
		DWORD garbage = 0;
		FillConsoleOutputCharacter(_info.ConsoleHandle, (TCHAR)' ', dwConSize, { 0,0 }, &garbage);
		_info.ConsolePos = { 0,0 };
	}
}