#pragma once
#include <vector>
#include <string>
#include "Guid.h"

namespace Protostar
{
	class CLogWindow
	{
	public: 
		CLogWindow();
		~CLogWindow();
		void OnRender();
	private:
		void RegisterToLog();
		std::vector<std::pair<struct SColor, std::string>> m_logBuffer;
		size_t m_lastFrameVectorSize;
		SGuid m_guid;
		bool m_bStopScroll;
	};
}