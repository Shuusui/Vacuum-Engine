#pragma once
#include "GlobalDefinitions.h"
#include "Array.h"
#include <functional>
#include "CoreSystems.h"

namespace Protostar::Core
{
constexpr u8 KEYMAP_MAX = 254;

	class Window;
	class CORESYSTEMS_API InputProcessor
	{
	public:
		InputProcessor(Window& _owningWindow);
		void RegisterInputDownCallback(u8 _key, const std::function<void()>& _func);
		void RegisterInputUpCallback(u8 _key, const std::function<void()>& _func);
	private:
		Window& m_owningWindow;
		s32 OnKeyDown(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);
		s32 OnLButtonDown(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);
		s32 OnRButtonDown(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);
		s32 OnMButtonDown(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);
		s32 OnXButtonDown(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);
		s32 OnKeyUp(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);
		s32 OnLButtonUp(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);
		s32 OnRButtonUp(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);
		s32 OnMButtonUp(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);
		s32 OnXButtonUp(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);

		FixedSizeArray<Array<std::function<void()>>, KEYMAP_MAX> m_keyDownMap;
		FixedSizeArray<Array<std::function<void()>>, KEYMAP_MAX> m_keyUpMap;
		FixedSizeArray<u64, 4> m_keysBitMap;
		u32 m_mouseKeysBitMap;
	};
}