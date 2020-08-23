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

void Vacuum::CTimer::OnUpdate()
{
	if (s_timer)
	{
		s_timer->Update();
	}
}

Vacuum::CTimer::CTimer()
	:m_ticksPerSecond(0)
	,m_time(0)
	,m_deltaSeconds(0.0f)
{
}

Vacuum::CTimer::~CTimer()
{
}

void Vacuum::CTimer::Update()
{
	int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	m_deltaSeconds = (float)(currentTime - m_time) / m_ticksPerSecond;
	m_time = currentTime;
}

bool Vacuum::CTimer::Init()
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
