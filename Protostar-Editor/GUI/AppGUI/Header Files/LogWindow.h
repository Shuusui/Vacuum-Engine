#pragma once
#include <vector>
#include <string>
#include "Guid.h"

namespace Protostar
{
	class PLogWindow
	{
	public: 
		PLogWindow();
		~PLogWindow();
		void OnRender();
	private:
		void RegisterToLog();
		std::vector<std::pair<struct PColor, std::string>> m_logBuffer;
		size_t m_lastFrameVectorSize;
		PGuid m_guid;
		bool m_bStopScroll;
	};
}