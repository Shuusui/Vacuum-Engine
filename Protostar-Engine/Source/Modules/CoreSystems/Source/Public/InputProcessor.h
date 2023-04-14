#pragma once
#include "GlobalDefinitions.h"
#include "Array.h"
#include "MathExt.h"
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
		s32 OnMouseWheel(void* _hwnd, u32 _msg, s64 _wParam, s64 _lParam);

		FixedSizeArray<Array<std::function<void()>>, KEYMAP_MAX> m_keyMap;
	};
}