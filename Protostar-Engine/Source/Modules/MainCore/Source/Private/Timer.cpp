#include "Timer.h"

namespace Protostar::Core
{
	Timer::Timer()
		:m_ticksPerSecond(0)
		, m_time(std::clock())
		, m_deltaSeconds(0.0f)
	{
	}

	s64 Timer::GetTicksPerSecond()  const
	{
		return m_ticksPerSecond;
	}

	s64 Timer::GetTime() const
	{
		return m_time;
	}

	float Timer::GetDeltaSeconds() const
	{
		return m_deltaSeconds;
	}

	void Timer::Update()
	{
		s64 currentTime = std::clock();
		s64 tickDiff = currentTime - m_time;
		m_deltaSeconds = static_cast<float>(tickDiff) / CLOCKS_PER_SEC;
		m_ticksPerSecond = static_cast<s64>(60 / m_deltaSeconds);
		m_time = currentTime;
	}
}