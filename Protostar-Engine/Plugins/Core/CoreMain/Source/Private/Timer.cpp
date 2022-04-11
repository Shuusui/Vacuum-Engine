#include "Timer.h"


Protostar::PTimer* Protostar::PTimer::s_timer = nullptr;

bool Protostar::PTimer::Create()
{
	if (s_timer)
	{
		return false;
	}
	s_timer = new PTimer();
	return true;
}

s64 Protostar::PTimer::GetTicksPerSecond()
{
	if (!s_timer)
	{
		return -1;
	}
	return s_timer->m_ticksPerSecond;
}

s64 Protostar::PTimer::GetTime()
{
	if (!s_timer)
	{
		return -1;
	}
	return s_timer->m_time;
}

float Protostar::PTimer::GetDeltaSeconds()
{
	if (!s_timer)
	{
		return -1.0f;
	}
	return s_timer->m_deltaSeconds;
}

void Protostar::PTimer::OnUpdate()
{
	if (s_timer)
	{
		s_timer->Update();
	}
}

Protostar::PTimer::PTimer()
	:m_ticksPerSecond(0)
	,m_time(std::clock())
	,m_deltaSeconds(0.0f)
{
}

void Protostar::PTimer::Update()
{
	clock_t currentTime = std::clock();
	clock_t tickDiff = currentTime - m_time;
	m_deltaSeconds = static_cast<float>(tickDiff) / CLOCKS_PER_SEC;
	m_ticksPerSecond = static_cast<clock_t>(60 / m_deltaSeconds);
	m_time = currentTime;
}
