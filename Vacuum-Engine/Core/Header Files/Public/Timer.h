#pragma once
#include <Windows.h>
#include "GlobalDefs.h"

namespace Vacuum
{
	class CTimer
	{
	public:
		static bool Create();
		static int64 GetTicksPerSecond();
		static int64 GetTime();
		static float GetDeltaSeconds();
		static void OnUpdate();
		CTimer();
		~CTimer();
		void Update();
		bool Init();
	private:
		static CTimer* s_timer;

		int64 m_ticksPerSecond;
		int64 m_time;
		float m_deltaSeconds;
	};
}