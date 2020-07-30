#pragma once

#pragma region Internal Includes 
#include "GlobalDefs.h"
#pragma endregion //Internal Includes
#pragma region External Includes
#include <Windows.h>
#include <utility>
#include <string>
#pragma endregion //External Includes

struct SWindowClassParams
{
	UINT m_style;
	HINSTANCE m_hInstance;
	LPCWSTR m_className;
	HBRUSH m_backgroundColor;
};

struct SWindowSizeParams
{
	LONG m_width;
	LONG m_height;
};

struct SWindowCreationParams
{
	DWORD m_dwExStyle;
	LPCWSTR m_windowName;
	HWND m_parentWindow;
	HMENU m_menu;
	LPVOID m_lpParam;
};

struct SWindowInfo
{
	SWindowClassParams m_classParams;
	SWindowSizeParams m_sizeParams;
	SWindowCreationParams m_creationParams;
	HWND m_wndHandle;
};

namespace Vacuum
{
	namespace Core
	{
		class CWindow
		{
		public:
			CWindow() = delete;

			CWindow(const CWindow&) = delete;

			CWindow(SWindowInfo&& _windowInfo)
				:m_windowInfo(std::move(_windowInfo))
			{
			}

			bool Create(std::wstring& _errorMsg);

			void ShwoAndUpdate(const int32& _nCmdShow);

			void RunWindow(MSG& _msg);

		private:
			SWindowInfo m_windowInfo;
		};
	}
}