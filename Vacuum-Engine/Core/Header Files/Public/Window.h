#pragma once

#pragma region Internal Includes 
#include "GlobalDefs.h"
#include "SharedStructs.h"
#pragma endregion //Internal Includes
#pragma region External Includes
#include <Windows.h>
#include <utility>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#pragma endregion //External Includes


namespace Vacuum
{
	struct SWindowClassParams
	{
		SWindowClassParams() = default;

		SWindowClassParams(const SWindowClassParams& _other)
			:Style(_other.Style)
			,HInstance(_other.HInstance)
			,ClassName(_other.ClassName)
			,BackgroundColor(_other.BackgroundColor)
		{
		}

		SWindowClassParams(SWindowClassParams&& _other) noexcept
			:Style(std::move(_other.Style))
			, HInstance(_other.HInstance)
			, ClassName(std::move(_other.ClassName))
			, BackgroundColor(std::move(_other.BackgroundColor))
		{
			_other.HInstance = nullptr;
		}

		UINT Style;
		HINSTANCE HInstance;
		LPCWSTR ClassName;
		HBRUSH BackgroundColor;


	};

	struct SWindowCreationParams
	{
		SWindowCreationParams() = default;

		SWindowCreationParams(const SWindowCreationParams& _other)
			:DwExStyle(_other.DwExStyle)
			,DwStyle(_other.DwStyle)
			,WindowName(_other.WindowName)
			,ParentWindow(_other.ParentWindow)
			,Menu(_other.Menu)
			,LpParam(_other.LpParam)
		{
		}

		SWindowCreationParams(SWindowCreationParams&& _other) noexcept
			:DwExStyle(std::move(_other.DwExStyle))
			, DwStyle(std::move(_other.DwStyle))
			, WindowName(std::move(_other.WindowName))
			, ParentWindow(std::move(_other.ParentWindow))
			, Menu(std::move(_other.Menu))
			, LpParam(std::move(_other.LpParam))
		{
			_other.ParentWindow = nullptr;
			_other.Menu = nullptr;
			_other.LpParam = nullptr;
		}

		DWORD DwExStyle;
		DWORD DwStyle;
		LPCWSTR WindowName;
		HWND ParentWindow;
		HMENU Menu;
		LPVOID LpParam;
	};

	struct SWindowInfo
	{
		SWindowInfo() = default;

		SWindowInfo(const SWindowInfo& _other)
			:ClassParams(_other.ClassParams)
			,DimParams(_other.DimParams)
			,CreationParams(_other.CreationParams)
		{
		}

		SWindowInfo(SWindowInfo&& _other) noexcept
			:ClassParams(std::move(_other.ClassParams))
			,DimParams(std::move(_other.DimParams))
			,CreationParams(std::move(_other.CreationParams))
		{
		}

		SWindowClassParams ClassParams;
		SWindowDimParams DimParams;
		SWindowCreationParams CreationParams;
	};

	class CMainWindow
	{
	public:
		static void InitWindow(const SWindowInfo& _windowInfo);

		static bool Create(std::string& _errorMsg);

		static void ShowAndUpdate(const s32& _nCmdShow);

		static bool RunWindow(MSG& _msg);

		static CMainWindow* GetWindowHandle();

		void UpdateWindowPos(const s32& _x, const s32& _y);

		void UpdateWindowSize(const s32& _width, const s32& _height);

		void RegisterCallbackForWMEvents(const u32& _wmEvent, const std::function<s32(HWND, u32, WPARAM, LPARAM)>& _func);

		std::vector<std::function<s32(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)>>& GetWMFunctions(u32 _wmEvent) const;

		HWND GetHwnd() const;

		SWindowDimParams GetCurrentDim() const;

	private:
		CMainWindow(const SWindowInfo& _windowInfo)
			:m_windowInfo(_windowInfo)
			,m_wndHandle(nullptr)
		{
			m_wmEventMap = {
				{WM_EXITSIZEMOVE, {}}, 
				{WM_LBUTTONDOWN, {}},
				{WM_RBUTTONDOWN, {}},
				{WM_LBUTTONUP, {}},
				{WM_RBUTTONUP, {}},
				{WM_SETCURSOR, {}},
				{WM_DESTROY, {}},
				{WM_WINDOWPOSCHANGED, {}},
				{WM_PAINT, {}},
				{WM_SIZE, {}},
				{WM_KEYDOWN, {}},
				{WM_KEYUP, {}},
				{WM_CHAR,{}}
			};
		}

		SWindowInfo m_windowInfo;
		HWND m_wndHandle;
		static CMainWindow* s_mainWindow;
		std::unordered_map<u32, std::vector<std::function<s32(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)>>> m_wmEventMap;
	};
}