#include "..\Header Files\Public\Timer.h"

Protostar::CTimer* Protostar::CTimer::s_timer = nullptr;

bool Protostar::CTimer::Create()
{
	if (s_timer)
	{
		return false;
	}
	s_timer = new CTimer();
	s_timer->Init();
	return true;
}

s64 Protostar::CTimer::GetTicksPerSecond()
{
	if (!s_timer)
	{
		return -1;
	}
	return s_timer->m_ticksPerSecond;
}

s64 Protostar::CTimer::GetTime()
{
	if (!s_timer)
	{
		return -1;
	}
	return s_timer->m_time;
}

float Protostar::CTimer::GetDeltaSeconds()
{
	if (!s_timer)
	{
		return -1.0f;
	}
	return s_timer->m_deltaSeconds;
}

void Protostar::CTimer::OnUpdate()
{
	if (s_timer)
	{
		s_timer->Update();
	}
}

Protostar::CTimer::CTimer()
	:m_ticksPerSecond(0)
	,m_time(0)
	,m_deltaSeconds(0.0f)
{
}

Protostar::CTimer::~CTimer()
{
}

void Protostar::CTimer::Update()
{
	s64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	m_deltaSeconds = (float)(currentTime - m_time) / m_ticksPerSecond;
	m_time = currentTime;
}

bool Protostar::CTimer::Init()
{

	if (!QueryPerformanceCounter((LARGE_INTEGER*)&m_time))
	{
		return false;
	}
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&m_ticksPerSecond))
	{
		return false;
	}
	return true;
}
