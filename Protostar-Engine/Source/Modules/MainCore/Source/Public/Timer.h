#pragma once

#include "GlobalDefinitions.h"
#include "MainCore.h"
#include <ctime>

namespace Protostar::Core
{
	class MAINCORE_API Timer
	{
	public:
		Timer();
		s64 GetTicksPerSecond() const;
		s64 GetTime() const;
		float GetDeltaSeconds() const;
		void Update();
	private:
		s64 m_ticksPerSecond;
		s64 m_time;
		float m_deltaSeconds;
	};
}
