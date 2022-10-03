#include "Window.h"

#include "Logger.h"
#include "Util.h"

namespace Protostar::Core
{
	constexpr LPCWSTR WINDOW_CLASS_NAME = L"WINDOW_CLASS";

	LRESULT CALLBACK WindowProc(
		HWND _hwnd,
		UINT _msg,
		WPARAM _wParam,
		LPARAM _lParam)
	{
		s32 returnValue = 0;
		Window* window = static_cast<Window*>(GetProp(_hwnd, WINDOW_CLASS_NAME));

		switch (_msg)
		{
		case WM_SIZE:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_SIZE))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_EXITSIZEMOVE:
		{
			if (window)
			{
				RECT rect = {};
				if (!GetWindowRect(_hwnd, &rect))
				{
					return -1;
				}
				window->UpdateWindowSize(rect.right - rect.left, rect.bottom - rect.top);
				window->UpdateWindowPos(rect.left, rect.top);
				for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_EXITSIZEMOVE))
				{
					s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
					if (tempReturn != 0)
					{
						returnValue = tempReturn;
					}
				}
			}
			return returnValue;
		}
		case WM_SETCURSOR:
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_SETCURSOR))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		case WM_PAINT:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_PAINT))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_LBUTTONDOWN))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_LBUTTONDBLCLK:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_LBUTTONDBLCLK))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_RBUTTONDOWN:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_RBUTTONDOWN))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_RBUTTONDBLCLK:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_RBUTTONDBLCLK))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MBUTTONDOWN:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_MBUTTONDOWN))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MBUTTONDBLCLK:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_MBUTTONDBLCLK))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_XBUTTONDOWN:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_XBUTTONDOWN))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_XBUTTONDBLCLK:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_XBUTTONDBLCLK))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_LBUTTONUP:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_LBUTTONUP))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_RBUTTONUP:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_RBUTTONUP))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MBUTTONUP:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_MBUTTONUP))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_XBUTTONUP:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_XBUTTONUP))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MOUSEWHEEL:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_MOUSEWHEEL))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MOUSEHWHEEL:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_MOUSEHWHEEL))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_KEYDOWN))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_SYSKEYDOWN))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_KEYUP))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_SYSKEYUP))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_CHAR:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_CHAR))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_DEVICECHANGE:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_DEVICECHANGE))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_DESTROY:
		{
			for (const std::function<s32(HWND, u32, WPARAM, LPARAM)>& func : window->GetEventFunctions(WM_DESTROY))
			{
				s32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			PostQuitMessage(0);
			return returnValue;
		}
		}
		return DefWindowProc(_hwnd, _msg, _wParam, _lParam);
	}

	bool Window::Create(std::string& _errorMsg)
	{
		WNDCLASSEX wndClass = {};
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = m_windowInfo.ClassParams.Style;
		wndClass.hInstance = m_windowInfo.ClassParams.HInstance;
		wndClass.lpszClassName = m_windowInfo.ClassParams.ClassName;
		wndClass.hbrBackground = m_windowInfo.ClassParams.BackgroundColor;
		wndClass.lpfnWndProc = WindowProc;

		RECT rect = {};
		rect.right = (LONG)m_windowInfo.DimParams.Width;
		rect.bottom = (LONG)m_windowInfo.DimParams.Height;
		if (!AdjustWindowRectEx(&rect, m_windowInfo.ClassParams.Style, false, NULL))
		{
			_errorMsg = PRINTF("Couldn't adjust window rect of window with classname %w", m_windowInfo.ClassParams.ClassName);
			return false;
		}
		if (!RegisterClassEx(&wndClass))
		{
			_errorMsg = PRINTF("Couldn't register wnd class with error: %w", GetLastErrorString().c_str());
			return false;
		}
		m_wndHandle = CreateWindowEx(
			static_cast<DWORD>(m_windowInfo.CreationParams.DwExStyle),
			m_windowInfo.ClassParams.ClassName,
			m_windowInfo.CreationParams.WindowName,
			static_cast<DWORD>(m_windowInfo.CreationParams.DwStyle),
			m_windowInfo.DimParams.LeftTopCornerX,
			m_windowInfo.DimParams.LeftTopCornerY,
			rect.right - rect.left,
			rect.bottom - rect.top,
			m_windowInfo.CreationParams.ParentWindow,
			m_windowInfo.CreationParams.Menu,
			m_windowInfo.ClassParams.HInstance,
			m_windowInfo.CreationParams.LpParam
		);
		if (!m_wndHandle)
		{
			_errorMsg = PRINTF("Couldn't create window with error %w", GetLastErrorString().c_str());
			return false;
		}
		
		SetProp(m_wndHandle, WINDOW_CLASS_NAME, static_cast<HANDLE>(this));
		return true;
	}

	void Window::ShowWindow(const s32 _nCmdShow)
	{
		::ShowWindow(m_wndHandle, _nCmdShow);
	}

	void Window::UpdateWindow()
	{
		::UpdateWindow(m_wndHandle);
	}

	bool Window::Run(MSG& _msg)
	{
		if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
			return true;
		}
		return false;
	}

	void Window::UpdateWindowPos(const s32 _x, const s32 _y)
	{
		m_windowInfo.DimParams.LeftTopCornerX = _x;
		m_windowInfo.DimParams.LeftTopCornerY = _y;
	}

	void Window::UpdateWindowSize(const s32 _width, const s32 _height)
	{
		m_windowInfo.DimParams.Width = _width;
		m_windowInfo.DimParams.Height = _height;
	}

	void Window::RegisterEventCallback(const u32 _wmEvent, const std::function<s32(HWND, u32, WPARAM, LPARAM)>& _func)
	{
		m_eventMap[_wmEvent].push_back(_func);
	}

	HWND Window::GetWindowHandle() const
	{
		return m_wndHandle;
	}

	WindowDimParams Window::GetCurrentDim() const
	{
		return m_windowInfo.DimParams;
	}
}
