#pragma once
#include <Windows.h>
#include "GlobalDefs.h"

namespace Vacuum
{
	class CTimer
	{
	public:
		static bool Create();
		static s64 GetTicksPerSecond();
		static s64 GetTime();
		static float GetDeltaSeconds();
		static void OnUpdate();
		CTimer();
		~CTimer();
		void Update();
		bool Init();
	private:
		static CTimer* s_timer;

		s64 m_ticksPerSecond;
		s64 m_time;
		float m_deltaSeconds;
	};
}