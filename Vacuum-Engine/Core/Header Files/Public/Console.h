#pragma once
#include <Windows.h>
#include "Guid.h"
#include "Log.h"

namespace Vacuum
{
	struct SConsoleInfo
	{
		COORD m_consolePos;
		HANDLE m_consoleHandle;

		SConsoleInfo()
			:m_consolePos({0,0})
			,m_consoleHandle(nullptr)
		{
		}

		SConsoleInfo(const SConsoleInfo& _other)
			:m_consolePos(_other.m_consolePos)
			,m_consoleHandle(_other.m_consoleHandle)
		{

		}

		SConsoleInfo(SConsoleInfo&& _other) noexcept
			:m_consolePos(std::move(_other.m_consolePos))
			,m_consoleHandle(std::move(_other.m_consoleHandle))
		{
			_other.m_consoleHandle = nullptr;
			_other.m_consolePos = {};
		}

	};

	struct SConsoleHandles
	{
		SConsoleInfo m_inputConInfo;
		SConsoleInfo m_outputConInfo;
		SConsoleInfo m_errorConInfo;
		SGuid m_handlesGuid;
	};

	/**
		* Allocates a console for this process and returns the handles to them with some info about it
		* @param _outHandles A reference to the Console handles struct with some info in it.
		* @return Returns true if everything was successful, false otherwise
		*/
	static bool AllocateConsole(SConsoleHandles& _outHandles)
	{
		bool success = AllocConsole();
				
		_outHandles.m_inputConInfo.m_consoleHandle = GetStdHandle(STD_INPUT_HANDLE);
		_outHandles.m_outputConInfo.m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		_outHandles.m_errorConInfo.m_consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
		_outHandles.m_handlesGuid = SGuid::NewGuid();
		return success && _outHandles.m_inputConInfo.m_consoleHandle && _outHandles.m_outputConInfo.m_consoleHandle && _outHandles.m_errorConInfo.m_consoleHandle;
	}

	static bool DeallocateConsole(SConsoleHandles& _handles)
	{
		FreeConsole();

		if (!CloseHandle(_handles.m_inputConInfo.m_consoleHandle))
		{
#if defined(_DEBUG)
			VE_DEBUG_LOG(TEXT("Failed to close input console handle"));
#endif
		}
		if (!CloseHandle(_handles.m_outputConInfo.m_consoleHandle))
		{
#if defined(_DEBUG)
			VE_DEBUG_LOG(TEXT("Failed to close output console handle"));
#endif
		}
		if (!CloseHandle(_handles.m_errorConInfo.m_consoleHandle))
		{
#if defined(_DEBUG)
			VE_DEBUG_LOG(TEXT("Failed to close error console handle"));
#endif
		}
	}
}