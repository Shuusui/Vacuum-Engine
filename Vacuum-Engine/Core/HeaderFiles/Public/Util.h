#pragma once
#include <string>
#include <Windows.h>
#include <strsafe.h>

#include "GlobalDefs.h"

namespace Vacuum
{
	namespace Core
	{
		/**
		 * A const char* to wchar_t* formatting
		 * @param str The string which should get formatted to a wchar_t*
		 * @return The formatted wchar_t* if it was successful or nullptr
		 */
		static wchar_t* ToWString(const char* _str)
		{
			size_t size = strlen(_str) + 1;
			wchar_t* returnStr = new wchar_t[size];
			size_t outSize; 
			mbstowcs_s(&outSize, returnStr, size, _str, size-1);
			return returnStr;
		}

		/**
		 * Printf formatting for a string 
		 * @param _string The initial string which will get formatted
		 * @param ... The variadic params which should get inserted in the string
		 * @return The formatted string
		 */
		static std::wstring Printf(std::wstring _string, ...)
		{
			auto strAddFunc = [&_string](const std::wstring& _tempStr, size_t& _index)->void
			{
				if (_string.max_size() < _string.size() + _tempStr.size())
				{
					_string.reserve(_string.size() + _tempStr.size());
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
					switch (_string[i+1])
					{
					case 'i':
					case 'd':
						strAddFunc(std::to_wstring(va_arg(args, int32)), i);
						break;
					case 'f':
						strAddFunc(std::to_wstring(va_arg(args, float)), i);
						break;
					case 'u':
						strAddFunc(std::to_wstring(va_arg(args, uint32)), i);
						break;
					case 'w':
						strAddFunc(va_arg(args, const wchar_t*), i);
						break;
					case 's':
						strAddFunc(ToWString(va_arg(args, const char*)), i);
						break;
					}
				}
			}
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
}