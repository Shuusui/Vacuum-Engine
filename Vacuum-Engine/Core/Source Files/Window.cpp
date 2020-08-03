#include "..\Header Files\Public\Window.h"

#pragma region Internal Includes 
#include "..\Header Files\Public\Log.h"
#include "..\Header Files\Public\Util.h"
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
		switch (_msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_WINDOWPOSCHANGED:
		{
			CMainWindow* mainWindow = CMainWindow::GetWindowHandle();
			if (mainWindow)
			{
				RECT rect = {};
				GetWindowRect(_hwnd, &rect);
				mainWindow->UpdateWindowPos(rect.left, rect.top);
			}
		}
			return 0;
		case WM_EXITSIZEMOVE:
		{
			CMainWindow* mainWindow = CMainWindow::GetWindowHandle();
			if (mainWindow)
			{
				RECT rect = {};
				GetWindowRect(_hwnd, &rect);
				mainWindow->UpdateWindowSize(rect.right - rect.left, rect.bottom - rect.top);
			}
			return 0;
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
		wndClass.style = s_mainWindow->m_windowInfo.m_classParams.m_style;
		wndClass.hInstance = s_mainWindow->m_windowInfo.m_classParams.m_hInstance;
		wndClass.lpszClassName = s_mainWindow->m_windowInfo.m_classParams.m_className;
		wndClass.hbrBackground = s_mainWindow->m_windowInfo.m_classParams.m_backgroundColor;
		wndClass.lpfnWndProc = WindowProc;
		RECT rect = {};
		rect.right = (LONG)s_mainWindow->m_windowInfo.m_dimParams.m_width;
		rect.bottom = (LONG)s_mainWindow->m_windowInfo.m_dimParams.m_height;
		if (!AdjustWindowRectEx(&rect, s_mainWindow->m_windowInfo.m_classParams.m_style, false, NULL))
		{
			_errorMsg = PRINTF("Couldn't adjust window rect of window with classname %w", s_mainWindow->m_windowInfo.m_classParams.m_className);
			return false;
		}
		if (!RegisterClassEx(&wndClass))
		{
			_errorMsg = PRINTF("Couldn't register wnd class with error: %w", GetLastErrorString().c_str());
			return false;
		}
		s_mainWindow->m_wndHandle = CreateWindowEx(
			s_mainWindow->m_windowInfo.m_creationParams.m_dwExStyle,
			s_mainWindow->m_windowInfo.m_classParams.m_className,
			s_mainWindow->m_windowInfo.m_creationParams.m_windowName,
			s_mainWindow->m_windowInfo.m_creationParams.m_dwStyle,
			s_mainWindow->m_windowInfo.m_dimParams.m_leftTopCornerX,
			s_mainWindow->m_windowInfo.m_dimParams.m_leftTopCornerY,
			rect.right - rect.left,
			rect.bottom - rect.top,
			s_mainWindow->m_windowInfo.m_creationParams.m_parentWindow,
			s_mainWindow->m_windowInfo.m_creationParams.m_menu,
			s_mainWindow->m_windowInfo.m_classParams.m_hInstance,
			s_mainWindow->m_windowInfo.m_creationParams.m_lpParam
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

	void CMainWindow::RunWindow(MSG& _msg)
	{
		if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
	}

	CMainWindow* CMainWindow::GetWindowHandle()
	{
		return s_mainWindow;
	}

	void CMainWindow::UpdateWindowPos(const int32& _x, const int32& _y)
	{
		m_windowInfo.m_dimParams.m_leftTopCornerX = _x;
		m_windowInfo.m_dimParams.m_leftTopCornerY = _y;
	}

	void CMainWindow::UpdateWindowSize(const int32& _width, const int32& _height)
	{
		m_windowInfo.m_dimParams.m_width = _width;
		m_windowInfo.m_dimParams.m_height = _height;
	}

	HWND CMainWindow::GetHwnd() const
	{
		return m_wndHandle;
	}

	SWindowDimParams CMainWindow::GetCurrentDim() const 
	{
		return m_windowInfo.m_dimParams;
	}
}
