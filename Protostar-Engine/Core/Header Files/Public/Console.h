#pragma once
#include <Windows.h>
#include "Guid.h"
#include "Log.h"

namespace Protostar
{
	struct SConsoleInfo
	{
		COORD ConsolePos;
		HANDLE ConsoleHandle;

		SConsoleInfo()
			: ConsolePos({ 0,0 })
			, ConsoleHandle(nullptr)
		{
		}

		SConsoleInfo(const SConsoleInfo& _other)
			: ConsolePos(_other.ConsolePos)
			, ConsoleHandle(_other.ConsoleHandle)
		{

		}

		SConsoleInfo(SConsoleInfo&& _other) noexcept
			: ConsolePos(std::move(_other.ConsolePos))
			, ConsoleHandle(std::move(_other.ConsoleHandle))
		{
			_other.ConsoleHandle = nullptr;
			_other.ConsolePos = {};
		}

	};

	struct SConsoleHandles
	{
		SConsoleInfo InputConInfo;
		SConsoleInfo OutputConInfo;
		SConsoleInfo ErrorConInfo;
		SGuid HandlesGuid;
	};

	/**
		* Allocates a console for this process and returns the handles to them with some info about it
		* @param _outHandles A reference to the Console handles struct with some info in it.
		* @return Returns true if everything was successful, false otherwise
		*/
	static bool AllocateConsole(SConsoleHandles& _outHandles)
	{
		bool success = AllocConsole();
				
		_outHandles.InputConInfo.ConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
		_outHandles.OutputConInfo.ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		_outHandles.ErrorConInfo.ConsoleHandle = GetStdHandle(STD_ERROR_HANDLE);
		_outHandles.HandlesGuid = SGuid::NewGuid();
		return success && _outHandles.InputConInfo.ConsoleHandle && _outHandles.OutputConInfo.ConsoleHandle && _outHandles.ErrorConInfo.ConsoleHandle;
	}

	static bool DeallocateConsole(SConsoleHandles& _handles)
	{
		FreeConsole();

		if (!CloseHandle(_handles.InputConInfo.ConsoleHandle))
		{
#if defined(_DEBUG)
			PE_DEBUG_LOG("Failed to close input console handle");
#endif
		}
		if (!CloseHandle(_handles.OutputConInfo.ConsoleHandle))
		{
#if defined(_DEBUG)
			PE_DEBUG_LOG("Failed to close output console handle");
#endif
		}
		if (!CloseHandle(_handles.ErrorConInfo.ConsoleHandle))
		{
#if defined(_DEBUG)
			PE_DEBUG_LOG("Failed to close error console handle");
#endif
		}
	}
}