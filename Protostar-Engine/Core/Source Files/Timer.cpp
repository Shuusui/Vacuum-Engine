#include "..\Header Files\Public\Timer.h"

Protostar::PTimer* Protostar::PTimer::s_timer = nullptr;

bool Protostar::PTimer::Create()
{
	if (s_timer)
	{
		return false;
	}
	s_timer = new PTimer();
	s_timer->Init();
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
	,m_time(0)
	,m_deltaSeconds(0.0f)
{
}

Protostar::PTimer::~PTimer()
{
}

void Protostar::PTimer::Update()
{
	s64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	m_deltaSeconds = (float)(currentTime - m_time) / m_ticksPerSecond;
	m_time = currentTime;
}

bool Protostar::PTimer::Init()
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
