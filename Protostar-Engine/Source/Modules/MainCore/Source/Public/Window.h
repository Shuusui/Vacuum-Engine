#pragma once

#include "GlobalDefinitions.h"
#include "MainCore.h"
#include "Vector.h"

#include <Windows.h>
#include <utility>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Protostar::Core
{
	struct MAINCORE_API WindowDimensions
	{
		Vector2D<s32> Size;
		Vector2D<s32> Position;
	};

	struct MAINCORE_API WindowClassParams
	{
		LPCWSTR ClassName;
		HINSTANCE HInstance;
		HBRUSH BackgroundColor;
		u32 Style;
	};

	struct MAINCORE_API WindowCreationParams
	{
		LPCWSTR WindowName;
		u64 DwExStyle;
		u64 DwStyle;
		HWND ParentWindow;
		HMENU Menu;
		LPVOID LpParam;
	};

	struct MAINCORE_API WindowInfo
	{
		WindowClassParams ClassParams;
		WindowDimensions DimParams;
		WindowCreationParams CreationParams;
	};

	class MAINCORE_API Window
	{
	public:
		void ShowWindow(const s32 _nCmdShow);

		void UpdateWindow();

		bool Run(MSG& _msg);

		void UpdateWindowPos(const s32 _x, const s32 _y);

		void UpdateWindowSize(const s32 _width, const s32 _height);

		void RegisterEventCallback(const u32 _wmEvent, const std::function<s32(HWND, u32, WPARAM, LPARAM)>& _func);

		HWND GetWindowHandle() const;

		const WindowDimensions& GetCurrentDimensions() const;

		const std::vector<std::function<s32(HWND, u32, WPARAM, LPARAM)>>& GetEventFunctions(u32 _eventKey) const
		{
			return m_eventMap.at(_eventKey);
		}
	private:
		friend class WindowManager;

		Window(const WindowInfo& _windowInfo)
			: m_windowInfo(_windowInfo)
			, m_wndHandle(nullptr)
			, m_eventMap({
				{WM_SIZE,{}},
				{WM_EXITSIZEMOVE,{}},
				{WM_SETCURSOR,{}},
				{WM_PAINT,{}},
				{WM_LBUTTONDOWN,{}},
				{WM_LBUTTONDBLCLK,{}},
				{WM_RBUTTONDOWN,{}},
				{WM_RBUTTONDBLCLK,{}},
				{WM_MBUTTONDOWN,{}},
				{WM_MBUTTONDBLCLK, {}},
				{WM_XBUTTONDOWN,{}},
				{WM_XBUTTONDBLCLK,{}},
				{WM_LBUTTONUP,{}},
				{WM_RBUTTONUP,{}},
				{WM_MBUTTONUP,{}},
				{WM_XBUTTONUP,{}},
				{WM_MOUSEWHEEL,{}},
				{WM_SYSKEYDOWN,{}},
				{WM_KEYDOWN,{}},
				{WM_SYSKEYUP, {}},
				{WM_KEYUP,{}},
				{WM_CHAR,{}},
				{WM_DEVICECHANGE,{}},
				{WM_DESTROY,{}},
				{WM_WINDOWPOSCHANGED,{}}
				})
		{
		}

		bool Create(std::string& _errorMsg);

		WindowInfo m_windowInfo;
		HWND m_wndHandle;
		std::unordered_map<u32, std::vector<std::function<s32(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)>>> m_eventMap;
	};
}
