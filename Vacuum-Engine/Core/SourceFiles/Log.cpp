#include "../HeaderFiles/Public/Log.h"
#include "../HeaderFiles/Public/Console.h"

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

		void CLog::RegisterHandle(const SGuid& _handleGuid, const SConsoleInfo& _outputInfo)
		{
			s_logHandle->m_logInfos.insert(std::make_pair(_handleGuid, _outputInfo));
		}

		void CLog::Log(const std::wstring& _logString)
		{
			for (std::pair<const SGuid, SConsoleInfo>& handlePair : s_logHandle->m_logInfos)
			{
				LogToHandle(handlePair.second, _logString);
			}
		}

		void CLog::ClearAllLogs()
		{
			for (std::pair<const SGuid, SConsoleInfo>& handlePair : s_logHandle->m_logInfos)
			{
				ClearLogHandle(handlePair.second);
			}
		}

		void CLog::Log(const SGuid& _handleGuid, const std::wstring& _logString)
		{
			LogToHandle(s_logHandle->m_logInfos.at(_handleGuid), _logString);
		}

		void CLog::ClearLog(const SGuid& _handleGuid)
		{
			ClearLogHandle(s_logHandle->m_logInfos.at(_handleGuid));
		}

		void CLog::Log(SGuid* _handleGuids, const size_t& _handleGuidAmount, const std::wstring& _logString)
		{
			for (int32 i = 0; i < _handleGuidAmount; ++i)
			{
				LogToHandle(s_logHandle->m_logInfos.at(_handleGuids[i]), _logString);
			}
		}

		void CLog::ClearLog(SGuid* _handleGuids, const size_t& _handleGuidAmount)
		{
			for (int32 i = 0; i < _handleGuidAmount; ++i)
			{
				ClearLogHandle(s_logHandle->m_logInfos.at(_handleGuids[i]));
			}
		}

		void CLog::LogToHandle(SConsoleInfo& _info, const std::wstring& _logString)
		{
			if (!_info.m_consoleHandle)
			{
				return;
			}
			CONSOLE_SCREEN_BUFFER_INFO screenBuf;
			GetConsoleScreenBufferInfo(_info.m_consoleHandle, &screenBuf);

			unsigned bufferSize = screenBuf.dwSize.X * screenBuf.dwSize.Y;

			CONSOLE_CURSOR_INFO cursor;
			cursor.dwSize = 1;
			cursor.bVisible = FALSE;
			SetConsoleCursorInfo(_info.m_consoleHandle, &cursor);

			DWORD garbage = 0;

			for (int32 i = 0; i < _logString.size(); ++i)
			{
				FillConsoleOutputCharacter(_info.m_consoleHandle, _logString[i], 1, _info.m_consolePos, &garbage);
				_info.m_consolePos.X++;
			}
			_info.m_consolePos.Y++;
			_info.m_consolePos.X = 0;
			SetConsoleCursorPosition(_info.m_consoleHandle, _info.m_consolePos);
			OutputDebugString(_logString.c_str());
			OutputDebugString(TEXT("\n"));
		}

		void CLog::ClearLogHandle(SConsoleInfo& _info)
		{
			if (!_info.m_consoleHandle)
			{
				return;
			}
			CONSOLE_SCREEN_BUFFER_INFO screenBuf;
			GetConsoleScreenBufferInfo(_info.m_consoleHandle, &screenBuf);
			DWORD dwConSize = screenBuf.dwSize.X * screenBuf.dwSize.Y;
			DWORD garbage = 0;
			FillConsoleOutputCharacter(_info.m_consoleHandle, (TCHAR)' ', dwConSize, { 0,0 }, &garbage);
			_info.m_consolePos = {0,0};
		}
	}
}