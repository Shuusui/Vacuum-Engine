#pragma once

#include "GlobalDefs.h"
#include "GlobalConstants.h"

import <string>;
#include <Windows.h>
#include <strsafe.h>

namespace Protostar
{
	/**
		* A const char* to wchar_t* formatting
		* @param str The string which should get formatted to a wchar_t*
		* @return The formatted wchar_t* if it was successful or nullptr
		*/
	static const char* ToString(const wchar_t* _wStr)
	{
		size_t outSize = wcslen(_wStr) + 1;
		char* outStr = new char[outSize];
		wcstombs_s(nullptr, outStr, outSize, _wStr, outSize - 1);
		return outStr;
	}

	/**
		* Printf formatting for a string 
		* @param _string The initial string which will get formatted
		* @param ... The variadic params which should get inserted in the string
		* @return The formatted string
		*/
	template<typename... Args>
	static std::string Printf(const std::string& _format, Args... args)
	{
		char tempStr[PRINTF_SMALL_BUFFER_SIZE] = {};
		const s32 writtenSize = std::snprintf(tempStr, 50, _format.c_str(), args...);
		if (writtenSize > 0)
		{
			return std::string(tempStr);
		}

		const s32 dynamicStrSize = std::snprintf(nullptr, 0, _format.c_str(), args...);
		char* dynamicTempStr = new char[dynamicStrSize];
		std::snprintf(dynamicTempStr, dynamicStrSize, _format.c_str(), args...);
		std::string returnStr = std::string(dynamicTempStr);
		delete[] dynamicTempStr;
		return returnStr;
	}

	/**
	* Gets the last error as a string representative
	* @return The error message of GetLastError() as std::wstring representative
	*/
	static std::wstring GetLastErrorString()
	{
		LPVOID lpMsgBuf;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER	|
						FORMAT_MESSAGE_FROM_SYSTEM			|
						FORMAT_MESSAGE_IGNORE_INSERTS		,
						nullptr								,
						GetLastError()						,
						MAKELANGID(
								LANG_NEUTRAL					,
								SUBLANG_DEFAULT				),
						(LPTSTR)&lpMsgBuf					,
						NULL								,
						nullptr								);

		return std::move((LPCTSTR)lpMsgBuf);
	}
}