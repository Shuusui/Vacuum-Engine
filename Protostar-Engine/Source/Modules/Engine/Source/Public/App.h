#pragma once
#include "Engine.h"
#include "Timer.h"
#include <string>
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <memory>

namespace Protostar::Core
{
	class ENGINE_API App
	{
	public:
		static bool Init(HINSTANCE _hInstance, s32 _nCmdShow, std::string& _errorMsg);
		static void Destroy();
		static App& Get()
		{
			return *s_app;
		}
		bool Update(MSG& msg) const;
	private:
		static inline App* s_app = nullptr;
		App();
		std::unique_ptr<Timer> m_timer;
	};
}