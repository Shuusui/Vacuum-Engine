#include "..\Header Files\Public\Timer.h"

Vacuum::CTimer* Vacuum::CTimer::s_timer = nullptr;

bool Vacuum::CTimer::Create()
{
	if (s_timer)
	{
		return false;
	}
	s_timer = new CTimer();
	s_timer->Init();
	return true;
}

int64 Vacuum::CTimer::GetTicksPerSecond()
{
	if (!s_timer)
	{
		return -1;
	}
	return s_timer->m_ticksPerSecond;
}

int64 Vacuum::CTimer::GetTime()
{
	if (!s_timer)
	{
		return -1;
	}
	return s_timer->m_time;
}

float Vacuum::CTimer::GetDeltaSeconds()
{
	if (!s_timer)
	{
		return -1.0f;
	}
	return s_timer->m_deltaSeconds;
}

void Vacuum::CTimer::Update()
{
}

bool Vacuum::CTimer::Init()
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&m_ticksPerSecond))
	{
		return false;
	}

	if (!QueryPerformanceCounter((LARGE_INTEGER*)&m_time))
	{
		return false;
	}
	return true;
}
