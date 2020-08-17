#include "../Header Files/Public/Log.h"
#include "../Header Files/Public/Console.h"

namespace Vacuum
{
	CLog* CLog::s_logHandle = nullptr;

	bool CLog::Init(std::wstring& _errorMsg)
	{
		if (s_logHandle)
		{
			_errorMsg = TEXT("Log handle is already initialized");
			return false;
		}
		s_logHandle = new CLog();
		return true;
	}

	void CLog::RegisterHandle(const SGuid& _handleGuid, const SConsoleInfo& _outputInfo)
	{
		s_logHandle->m_logMutex.lock();
		s_logHandle->m_logInfos.insert(std::make_pair(_handleGuid, _outputInfo));
		s_logHandle->m_logMutex.unlock();
	}

	void CLog::Log(const std::wstring& _logString)
	{
		s_logHandle->m_logMutex.lock();
		for (std::pair<const SGuid, SConsoleInfo>& handlePair : s_logHandle->m_logInfos)
		{
			LogToHandle(handlePair.second, _logString);
		}
		s_logHandle->m_logMutex.unlock();
	}

	void CLog::ClearAllLogs()
	{
		s_logHandle->m_logMutex.lock();
		for (std::pair<const SGuid, SConsoleInfo>& handlePair : s_logHandle->m_logInfos)
		{
			ClearLogHandle(handlePair.second);
		}
		s_logHandle->m_logMutex.unlock();
	}

	void CLog::Log(const SGuid& _handleGuid, const std::wstring& _logString)
	{
		s_logHandle->m_logMutex.lock();
		LogToHandle(s_logHandle->m_logInfos.at(_handleGuid), _logString);
		s_logHandle->m_logMutex.unlock();
	}

	void CLog::ClearLog(const SGuid& _handleGuid)
	{
		s_logHandle->m_logMutex.lock();
		ClearLogHandle(s_logHandle->m_logInfos.at(_handleGuid));
		s_logHandle->m_logMutex.unlock();
	}

	void CLog::Log(SGuid* _handleGuids, const size_t& _handleGuidAmount, const std::wstring& _logString)
	{
		s_logHandle->m_logMutex.lock();
		for (int32 i = 0; i < _handleGuidAmount; ++i)
		{
			LogToHandle(s_logHandle->m_logInfos.at(_handleGuids[i]), _logString);
		}
		s_logHandle->m_logMutex.unlock();
	}

	void CLog::LogDebugString(const std::wstring& _logString)
	{
		OutputDebugString(_logString.c_str());
		OutputDebugString(TEXT("\n"));
	}

	void CLog::ClearLog(SGuid* _handleGuids, const size_t& _handleGuidAmount)
	{
		s_logHandle->m_logMutex.lock();
		for (int32 i = 0; i < _handleGuidAmount; ++i)
		{
			ClearLogHandle(s_logHandle->m_logInfos.at(_handleGuids[i]));
		}
		s_logHandle->m_logMutex.unlock();
	}

	void CLog::LogToHandle(SConsoleInfo& _info, const std::wstring& _logString)
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

		for (int32 i = 0; i < _logString.size(); ++i)
		{
			FillConsoleOutputCharacter(_info.ConsoleHandle, _logString[i], 1, _info.ConsolePos, &garbage);
			_info.ConsolePos.X++;
		}
		_info.ConsolePos.Y++;
		_info.ConsolePos.X = 0;
		SetConsoleCursorPosition(_info.ConsoleHandle, _info.ConsolePos);
		OutputDebugString(_logString.c_str());
		OutputDebugString(TEXT("\n"));
	}

	void CLog::ClearLogHandle(SConsoleInfo& _info)
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