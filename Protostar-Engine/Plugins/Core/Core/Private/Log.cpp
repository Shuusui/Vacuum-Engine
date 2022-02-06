#include "../Header Files/Public/Log.h"
#include "../Header Files/Public/Console.h"

namespace Protostar
{
	PLog* PLog::s_logHandle = nullptr;

	bool PLog::Init(std::string& _errorMsg)
	{
		if (s_logHandle)
		{
			_errorMsg = "Log handle is already initialized";
			return false;
		}
		s_logHandle = new PLog();
		return true;
	}

	void PLog::RegisterHandle(const PGuid& _handleGuid, const PConsoleInfo& _outputInfo)
	{
		s_logHandle->m_logMutex.lock();
		s_logHandle->m_logInfos.insert(std::make_pair(_handleGuid, _outputInfo));
		s_logHandle->m_logMutex.unlock();
	}

	void PLog::RegisterBuffer(const PGuid& _bufGuid, std::vector<std::pair<PColor, std::string>>* buf)
	{
		*buf = s_logHandle->m_buffer;
		s_logHandle->m_logBuffer.insert(std::make_pair(_bufGuid, buf));
	}

	void PLog::RemoveBuffer(const PGuid& _bufGuid)
	{
		s_logHandle->m_logBuffer.erase(_bufGuid);
	}

	bool PLog::IsBufRegistered(const PGuid& _bufGuid)
	{
		return s_logHandle->m_logBuffer.find(_bufGuid) == s_logHandle->m_logBuffer.end();
	}

	void PLog::Log(const std::string& _logString)
	{
		s_logHandle->m_logMutex.lock();
		s_logHandle->m_buffer.push_back(std::make_pair(PColor{1.0f, 0.4f, 0.8f, 1.0f}, _logString));
		for (std::pair<const PGuid, PConsoleInfo>& handlePair : s_logHandle->m_logInfos)
		{
			LogToHandle(handlePair.second, _logString);
		}

		for (const std::pair<PGuid, std::vector<std::pair<PColor, std::string>>*>& bufPair : s_logHandle->m_logBuffer)
		{
			PColor color = { 1.0f, 0.4f, 0.8f, 1.0f };
			bufPair.second->push_back(std::make_pair(color, _logString));
		}

		OutputDebugStringA(_logString.c_str());
		OutputDebugStringA("\n");

		s_logHandle->m_logMutex.unlock();
	}

	void PLog::ClearAllLogs()
	{
		s_logHandle->m_logMutex.lock();
		for (std::pair<const PGuid, PConsoleInfo>& handlePair : s_logHandle->m_logInfos)
		{
			ClearLogHandle(handlePair.second);
		}
		s_logHandle->m_logMutex.unlock();
	}

	void PLog::Log(const PGuid& _handleGuid, const std::string& _logString)
	{
		s_logHandle->m_logMutex.lock();
		LogToHandle(s_logHandle->m_logInfos.at(_handleGuid), _logString);
		s_logHandle->m_logMutex.unlock();
	}

	void PLog::ClearLog(const PGuid& _handleGuid)
	{
		s_logHandle->m_logMutex.lock();
		ClearLogHandle(s_logHandle->m_logInfos.at(_handleGuid));
		s_logHandle->m_logMutex.unlock();
	}

	void PLog::Log(PGuid* _handleGuids, const size_t _handleGuidAmount, const std::string& _logString)
	{
		s_logHandle->m_logMutex.lock();
		for (s32 i = 0; i < _handleGuidAmount; ++i)
		{
			LogToHandle(s_logHandle->m_logInfos.at(_handleGuids[i]), _logString);
		}
		s_logHandle->m_logMutex.unlock();
	}

	void PLog::LogDebugString(const std::string& _logString)
	{
		OutputDebugStringA(_logString.c_str());
		OutputDebugStringA("\n");
	}

	void PLog::ClearLog(PGuid* _handleGuids, const size_t _handleGuidAmount)
	{
		s_logHandle->m_logMutex.lock();
		for (s32 i = 0; i < _handleGuidAmount; ++i)
		{
			ClearLogHandle(s_logHandle->m_logInfos.at(_handleGuids[i]));
		}
		s_logHandle->m_logMutex.unlock();
	}

	void PLog::LogToHandle(PConsoleInfo& _info, const std::string& _logString)
	{
		if (!_info.ConsoleHandle)
		{
			return;
		}
		CONSOLE_SCREEN_BUFFER_INFO screenBuf;
		GetConsoleScreenBufferInfo(_info.ConsoleHandle, &screenBuf);

		unsigned bufferSize = screenBuf.dwSize.X * screenBuf.dwSize.Y;

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

	void PLog::ClearLogHandle(PConsoleInfo& _info)
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
		_info.ConsolePos = {0,0};
	}
}