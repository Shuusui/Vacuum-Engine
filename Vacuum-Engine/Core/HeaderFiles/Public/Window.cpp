#include "Window.h"

#pragma region Internal Includes 
#include "Log.h"
#include "Util.h"
#pragma endregion //Internal Includes
#pragma region External Includes
#pragma endregion //External Includes

bool Vacuum::Core::CWindow::Create(std::wstring& _errorMsg)
{
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = m_windowInfo.m_classParams.m_style;
	wndClass.hInstance = m_windowInfo.m_classParams.m_hInstance;
	wndClass.lpszClassName = m_windowInfo.m_classParams.m_className;
	wndClass.hbrBackground = m_windowInfo.m_classParams.m_backgroundColor;
	RECT rect = {};
	rect.right = m_windowInfo.m_sizeParams.m_width;
	rect.bottom = m_windowInfo.m_sizeParams.m_height;
	if (!AdjustWindowRectEx(&rect, m_windowInfo.m_classParams.m_style, false, NULL))
	{
		_errorMsg = PRINTF("Couldn't adjust window rect of window with classname %w", m_windowInfo.m_classParams.m_className);
		return false;
	}
	if (!RegisterClassEx(&wndClass))
	{
		_errorMsg = PRINTF("Couldn't register wnd class with error: %w", GetLastErrorString().c_str());
		return false;
	}
	m_windowInfo.m_wndHandle = CreateWindowEx(
		m_windowInfo.m_creationParams.m_dwExStyle,
		m_windowInfo.m_classParams.m_className,
		m_windowInfo.m_creationParams.m_windowName,
		m_windowInfo.m_classParams.m_style,
		0,
		0,
		rect.right - rect.left, 
		rect.bottom - rect.top,
		m_windowInfo.m_creationParams.m_parentWindow,
		m_windowInfo.m_creationParams.m_menu,
		m_windowInfo.m_classParams.m_hInstance,
		m_windowInfo.m_creationParams.m_lpParam
	);
	if (!m_windowInfo.m_wndHandle)
	{
		_errorMsg = PRINTF("Couldn't create window with error %w", GetLastErrorString().c_str());
		return false;
	}
	true;
}

void Vacuum::Core::CWindow::ShwoAndUpdate(const int32& _nCmdShow)
{
	ShowWindow(m_windowInfo.m_wndHandle, _nCmdShow);
	UpdateWindow(m_windowInfo.m_wndHandle);
}

void Vacuum::Core::CWindow::RunWindow(MSG& _msg)
{
	if (PeekMessage(&_msg, m_windowInfo.m_wndHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
}

