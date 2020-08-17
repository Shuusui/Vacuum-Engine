#include "..\Header Files\Public\Window.h"

#pragma region Internal Includes 
#include "..\Header Files\Public\Log.h"
#include "..\Header Files\Public\Util.h"
#include "RendererManager.h"
#pragma endregion //Internal Includes
#pragma region External Includes
#pragma endregion //External Includes

namespace Vacuum
{
	CMainWindow* CMainWindow::s_mainWindow = nullptr;

	LRESULT CALLBACK WindowProc(
		HWND _hwnd,
		UINT _msg,
		WPARAM _wParam,
		LPARAM _lParam)
	{
		CMainWindow* mainWindow = CMainWindow::GetWindowHandle();
		int32 returnValue = 0;
		
		switch (_msg)
		{
		case WM_SIZE:
		{
			VE_DEBUG_LOG_F(TEXT("Message: %u"), _msg);
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_SIZE))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			break;
		}
		//case WM_WINDOWPOSCHANGED:
		//{
		//	VE_DEBUG_LOG_F(TEXT("Message: %u"), _msg);
		//	if (mainWindow)
		//	{
		//		for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_WINDOWPOSCHANGED))
		//		{
		//			int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
		//			if (tempReturn != 0)
		//			{
		//				returnValue = tempReturn;
		//			}
		//		}
		//		return returnValue;
		//		RECT rect = {};
		//		if (!GetWindowRect(_hwnd, &rect))
		//		{
		//			return -1;
		//		}
		//		mainWindow->UpdateWindowPos(rect.left, rect.top);
		//	}
		//	break;
		//}
		case WM_EXITSIZEMOVE:
		{
			if (mainWindow)
			{
				RECT rect = {};
				if (!GetWindowRect(_hwnd, &rect))
				{
					return -1;
				}
				mainWindow->UpdateWindowSize(rect.right - rect.left, rect.bottom - rect.top);
				for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_EXITSIZEMOVE))
				{
					int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
					if (tempReturn != 0)
					{
						returnValue = tempReturn;
					}
				}
			}
			return returnValue;
		}
		case WM_SETCURSOR:
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_SETCURSOR))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		case WM_PAINT:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_PAINT))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_LBUTTONDOWN))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_LBUTTONDBLCLK:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_LBUTTONDBLCLK))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_RBUTTONDOWN:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_RBUTTONDOWN))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_RBUTTONDBLCLK:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_RBUTTONDBLCLK))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MBUTTONDOWN: 
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_MBUTTONDOWN))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MBUTTONDBLCLK:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_MBUTTONDBLCLK))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_XBUTTONDOWN:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_XBUTTONDOWN))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_XBUTTONDBLCLK:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_XBUTTONDBLCLK))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_LBUTTONUP:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_LBUTTONUP))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_RBUTTONUP:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_RBUTTONUP))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MBUTTONUP:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_MBUTTONUP))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_XBUTTONUP:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_XBUTTONUP))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MOUSEWHEEL:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_MOUSEWHEEL))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_MOUSEHWHEEL:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_MOUSEHWHEEL))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
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
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_KEYDOWN))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_SYSKEYDOWN))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
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
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_KEYUP))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_SYSKEYUP))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_CHAR:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_CHAR))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_DEVICECHANGE:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_DEVICECHANGE))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
				if (tempReturn != 0)
				{
					returnValue = tempReturn;
				}
			}
			return returnValue;
		}
		case WM_DESTROY:
		{
			for (const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& func : mainWindow->GetWMFunctions(WM_DESTROY))
			{
				int32 tempReturn = func(_hwnd, _msg, _wParam, _lParam);
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

	void CMainWindow::InitWindow(const SWindowInfo& _windowInfo)
	{
		if (s_mainWindow)
		{
			return;
		}
		s_mainWindow = new CMainWindow(_windowInfo);
	}

	bool CMainWindow::Create(std::wstring& _errorMsg)
	{
		WNDCLASSEX wndClass = {};
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = s_mainWindow->m_windowInfo.ClassParams.Style;
		wndClass.hInstance = s_mainWindow->m_windowInfo.ClassParams.HInstance;
		wndClass.lpszClassName = s_mainWindow->m_windowInfo.ClassParams.ClassName;
		wndClass.hbrBackground = s_mainWindow->m_windowInfo.ClassParams.BackgroundColor;
		wndClass.lpfnWndProc = WindowProc;

		RECT rect = {};
		rect.right = (LONG)s_mainWindow->m_windowInfo.DimParams.Width;
		rect.bottom = (LONG)s_mainWindow->m_windowInfo.DimParams.Height;
		if (!AdjustWindowRectEx(&rect, s_mainWindow->m_windowInfo.ClassParams.Style, false, NULL))
		{
			_errorMsg = PRINTF("Couldn't adjust window rect of window with classname %s", s_mainWindow->m_windowInfo.ClassParams.ClassName);
			return false;
		}
		if (!RegisterClassEx(&wndClass))
		{
			_errorMsg = PRINTF("Couldn't register wnd class with error: %s", GetLastErrorString().c_str());
			return false;
		}
		s_mainWindow->m_wndHandle = CreateWindowEx(
			s_mainWindow->m_windowInfo.CreationParams.DwExStyle,
			s_mainWindow->m_windowInfo.ClassParams.ClassName,
			s_mainWindow->m_windowInfo.CreationParams.WindowName,
			s_mainWindow->m_windowInfo.CreationParams.DwStyle,
			s_mainWindow->m_windowInfo.DimParams.LeftTopCornerX,
			s_mainWindow->m_windowInfo.DimParams.LeftTopCornerY,
			rect.right - rect.left,
			rect.bottom - rect.top,
			s_mainWindow->m_windowInfo.CreationParams.ParentWindow,
			s_mainWindow->m_windowInfo.CreationParams.Menu,
			s_mainWindow->m_windowInfo.ClassParams.HInstance,
			s_mainWindow->m_windowInfo.CreationParams.LpParam
		);
		if (!s_mainWindow->m_wndHandle)
		{
			_errorMsg = PRINTF("Couldn't create window with error %w", GetLastErrorString().c_str());
			return false;
		}
		return true;
	}

	void CMainWindow::ShowAndUpdate(const int32& _nCmdShow)
	{
		ShowWindow(s_mainWindow->m_wndHandle, _nCmdShow);
		UpdateWindow(s_mainWindow->m_wndHandle);
	}

	bool CMainWindow::RunWindow(MSG& _msg)
	{
		if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
			return true;
		}
		return false;
	}

	CMainWindow* CMainWindow::GetWindowHandle()
	{
		return s_mainWindow;
	}

	void CMainWindow::UpdateWindowPos(const int32& _x, const int32& _y)
	{
		m_windowInfo.DimParams.LeftTopCornerX = _x;
		m_windowInfo.DimParams.LeftTopCornerY = _y;
	}

	void CMainWindow::UpdateWindowSize(const int32& _width, const int32& _height)
	{
		m_windowInfo.DimParams.Width = _width;
		m_windowInfo.DimParams.Height = _height;
	}

	void CMainWindow::RegisterCallbackForWMEvents(const uint32& _wmEvent, const std::function<int32(HWND, uint32, WPARAM, LPARAM)>& _func)
	{
		m_wmEventMap[_wmEvent].push_back(_func);
	}

	std::vector<std::function<int32(HWND, uint32, WPARAM, LPARAM)>>& CMainWindow::GetWMFunctions(uint32 _wmEvent) const
	{
		return s_mainWindow->m_wmEventMap[_wmEvent];
	}

	HWND CMainWindow::GetHwnd() const
	{
		return m_wndHandle;
	}

	SWindowDimParams CMainWindow::GetCurrentDim() const 
	{
		return m_windowInfo.DimParams;
	}
}
