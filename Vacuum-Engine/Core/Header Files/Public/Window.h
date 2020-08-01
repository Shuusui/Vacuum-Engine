#pragma once

#pragma region Internal Includes 
#include "GlobalDefs.h"
#include "SharedStructs.h"
#pragma endregion //Internal Includes
#pragma region External Includes
#include <Windows.h>
#include <utility>
#include <string>
#pragma endregion //External Includes


namespace Vacuum
{
	struct SWindowClassParams
	{
		SWindowClassParams() = default;

		SWindowClassParams(const SWindowClassParams& _other)
			:m_style(_other.m_style)
			,m_hInstance(_other.m_hInstance)
			,m_className(_other.m_className)
			,m_backgroundColor(_other.m_backgroundColor)
		{
		}

		SWindowClassParams(SWindowClassParams&& _other) noexcept
			:m_style(std::move(_other.m_style))
			, m_hInstance(_other.m_hInstance)
			, m_className(std::move(_other.m_className))
			, m_backgroundColor(std::move(_other.m_backgroundColor))
		{
			_other.m_hInstance = nullptr;
		}

		UINT m_style;
		HINSTANCE m_hInstance;
		LPCWSTR m_className;
		HBRUSH m_backgroundColor;


	};

	struct SWindowCreationParams
	{
		SWindowCreationParams() = default;

		SWindowCreationParams(const SWindowCreationParams& _other)
			:m_dwExStyle(_other.m_dwExStyle)
			,m_dwStyle(_other.m_dwStyle)
			,m_windowName(_other.m_windowName)
			,m_parentWindow(_other.m_parentWindow)
			,m_menu(_other.m_menu)
			,m_lpParam(_other.m_lpParam)
		{
		}

		SWindowCreationParams(SWindowCreationParams&& _other) noexcept
			:m_dwExStyle(std::move(_other.m_dwExStyle))
			, m_dwStyle(std::move(_other.m_dwStyle))
			, m_windowName(std::move(_other.m_windowName))
			, m_parentWindow(std::move(_other.m_parentWindow))
			, m_menu(std::move(_other.m_menu))
			, m_lpParam(std::move(_other.m_lpParam))
		{
			_other.m_parentWindow = nullptr;
			_other.m_menu = nullptr;
			_other.m_lpParam = nullptr;
		}

		DWORD m_dwExStyle;
		DWORD m_dwStyle;
		LPCWSTR m_windowName;
		HWND m_parentWindow;
		HMENU m_menu;
		LPVOID m_lpParam;
	};

	struct SWindowInfo
	{
		SWindowInfo() = default;

		SWindowInfo(const SWindowInfo& _other)
			:m_classParams(_other.m_classParams)
			,m_dimParams(_other.m_dimParams)
			,m_creationParams(_other.m_creationParams)
		{
		}

		SWindowInfo(SWindowInfo&& _other) noexcept
			:m_classParams(std::move(_other.m_classParams))
			,m_dimParams(std::move(_other.m_dimParams))
			,m_creationParams(std::move(_other.m_creationParams))
		{
		}

		SWindowClassParams m_classParams;
		SWindowDimParams m_dimParams;
		SWindowCreationParams m_creationParams;
	};

	class CMainWindow
	{
	public:
		static void InitWindow(const SWindowInfo& _windowInfo);

		static bool Create(std::wstring& _errorMsg);

		static void ShowAndUpdate(const int32& _nCmdShow);

		static void RunWindow(MSG& _msg);

		static CMainWindow* GetWindowHandle();

		void UpdateWindowPos(const int32& _x, const int32& _y);

		void UpdateWindowSize(const int32& _width, const int32& _height);

		SWindowDimParams GetCurrentDim() const;

	private:
		CMainWindow(const SWindowInfo& _windowInfo)
			:m_windowInfo(_windowInfo)
			,m_wndHandle(nullptr)
		{
		}

		SWindowInfo m_windowInfo;
		HWND m_wndHandle;
		static CMainWindow* s_mainWindow;
	};
}