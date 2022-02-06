#ifndef TIMER_H
#define TIMER_H

#include "GlobalDefinitions.h"
#include <ctime>

namespace Protostar
{
	class PTimer
	{
	public:
		static bool Create();
		static s64 GetTicksPerSecond();
		static s64 GetTime();
		static float GetDeltaSeconds();
		static void OnUpdate();
		PTimer();
		void Update();
	private:
		static PTimer* s_timer;

		clock_t m_ticksPerSecond;
		clock_t m_time;
		float m_deltaSeconds;
	};
}
#endif //TIMER_H