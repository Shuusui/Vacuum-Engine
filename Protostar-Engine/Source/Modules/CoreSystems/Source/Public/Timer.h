#pragma once

#include "GlobalDefinitions.h"
#include <ctime>
#include "CoreSystems.h"

namespace Protostar::Core
{
	class CORESYSTEMS_API Timer
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
