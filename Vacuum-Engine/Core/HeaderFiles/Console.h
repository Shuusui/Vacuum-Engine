#pragma once
#include <Windows.h>

namespace Vacuum
{
	namespace Core
	{
		struct SConsoleHandles
		{
			HANDLE m_inputHandle;
			HANDLE m_outputHandle; 
			HANDLE m_errorHandle;
		};

		class CConsole
		{
		public:

			static bool AllocateConsole(SConsoleHandles& _outHandles)
			{
				bool success = AllocConsole();
				
				_outHandles.m_inputHandle = GetStdHandle(STD_INPUT_HANDLE);
				_outHandles.m_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				_outHandles.m_errorHandle = GetStdHandle(STD_ERROR_HANDLE);

				return success;
			}
		};
	}
}