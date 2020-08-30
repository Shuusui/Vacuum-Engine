#pragma once
#include <string>
#include <Windows.h>
#include <strsafe.h>

#include "GlobalDefs.h"

namespace Vacuum
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
	static std::string Printf(std::string _string, ...)
	{
		auto strAddFunc = [&_string](const std::string& _tempStr, size_t& _index)->void
		{
			if (_string.max_size() < _string.size() + _tempStr.size())
			{
				_string.reserve(_string.size() + _tempStr.size());
			}

			if (_tempStr.size() == 0)
			{
				_string.erase(_string.begin() + _index);
				_string.erase(_string.begin() + _index + 1);
				return;
			}

			if (_tempStr.size() == 1)
			{
				_string[_index] = _tempStr[0];
				_string.erase(_string.begin() + _index + 1);
				return;
			}

			for (size_t i = 0; i < _tempStr.size(); ++i)
			{
				
				if (i < 2)
				{
					_string[_index+i] = _tempStr[i];
					continue;
				}
				_string.insert(_string.begin() +_index+i, _tempStr[i]);
			}
			_index +=_tempStr.size();
		};
		va_list args; 
		va_start(args, _string);
		for (size_t i = 0; i < _string.size(); ++i)
		{
			std::wstring tempArg;
			if (_string[i] == '%')
			{
#pragma warning(disable : 6285)
				switch (_string[i+1])
				{
				case 'i':
				case 'd':
					strAddFunc(std::to_string(va_arg(args, s32)), i);
					break;
				case 'f':
					strAddFunc(std::to_string(va_arg(args, float)), i);
					break;
				case 'u':
					strAddFunc(std::to_string(va_arg(args, u32)), i);
					break;
				case 'w':
					strAddFunc(ToString(va_arg(args, const wchar_t*)), i);
					break;
				case 's':
					strAddFunc(va_arg(args, const char*), i);
					break;
				}
			}
		}
#pragma warning(default : 6285)
		va_end(args);
		return _string;
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