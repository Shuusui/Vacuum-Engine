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

		void CLog::Log(const std::wstring& _logString)
		{
			for (const std::pair<SGuid, HANDLE>& handlePair : s_logHandle->m_logHandles)
			{
				LogToHandle(handlePair.second, _logString);
			}
		}

		void CLog::Log(const SGuid& _handleGuid, const std::wstring& _logString)
		{
			LogToHandle(s_logHandle->m_logHandles.at(_handleGuid), _logString);
		}

		void CLog::Log(SGuid* _handleGuids, const size_t& _handleGuidAmount, const std::wstring& _logString)
		{
			for (int32 i = 0; i < _handleGuidAmount; ++i)
			{
				LogToHandle(s_logHandle->m_logHandles.at(_handleGuids[i]), _logString);
			}
		}

		void CLog::LogToHandle(const HANDLE& _handle, const std::wstring& _logString)
		{
			CONSOLE_SCREEN_BUFFER_INFO screenBuf;
			GetConsoleScreenBufferInfo(_handle, &screenBuf);

			unsigned bufferSize = screenBuf.dwSize.X * screenBuf.dwSize.Y;

			COORD start;
			start.X = 0;
			start.Y = 0;

			CONSOLE_CURSOR_INFO cursor;
			cursor.dwSize = 1;
			cursor.bVisible = FALSE;
			SetConsoleCursorInfo(_handle, &cursor);

			DWORD garbage = 0;

			for (int32 i = 0; i < _logString.size(); ++i)
			{
				FillConsoleOutputCharacter(_handle, _logString[i], 1, start, &garbage);
				start.X++;
			}
		}
	}
}