#pragma once
#include <string>
#include <Windows.h>
#include <strsafe.h>

namespace Vacuum
{
	namespace Core
	{
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