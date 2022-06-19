#pragma once

#include "GlobalDefinitions.h"
#include "SharedStructs.h"

#include <Windows.h>
#include <utility>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>


namespace Protostar
{
	struct PWindowClassParams
	{
		PWindowClassParams() = default;

		PWindowClassParams(const PWindowClassParams& _other)
			:Style(_other.Style)
			,HInstance(_other.HInstance)
			,ClassName(_other.ClassName)
			,BackgroundColor(_other.BackgroundColor)
		{
		}

		PWindowClassParams(PWindowClassParams&& _other) noexcept
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

	struct PWindowCreationParams
	{
		PWindowCreationParams() = default;

		PWindowCreationParams(const PWindowCreationParams& _other)
			:DwExStyle(_other.DwExStyle)
			,DwStyle(_other.DwStyle)
			,WindowName(_other.WindowName)
			,ParentWindow(_other.ParentWindow)
			,Menu(_other.Menu)
			,LpParam(_other.LpParam)
		{
		}

		PWindowCreationParams(PWindowCreationParams&& _other) noexcept
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

	struct PWindowInfo
	{
		PWindowInfo() = default;

		PWindowInfo(const PWindowInfo& _other)
			:ClassParams(_other.ClassParams)
			,DimParams(_other.DimParams)
			,CreationParams(_other.CreationParams)
		{
		}

		PWindowInfo(PWindowInfo&& _other) noexcept
			:ClassParams(std::move(_other.ClassParams))
			,DimParams(std::move(_other.DimParams))
			,CreationParams(std::move(_other.CreationParams))
		{
		}

		PWindowClassParams ClassParams;
		SWindowDimParams DimParams;
		PWindowCreationParams CreationParams;
	};

	class PMainWindow
	{
	public:
		static void InitWindow(const PWindowInfo& _windowInfo);

		static bool Create(std::string& _errorMsg);

		static void ShowAndUpdate(const s32 _nCmdShow);

		static bool RunWindow(MSG& _msg);

		static PMainWindow* GetWindowHandle();

		void UpdateWindowPos(const s32 _x, const s32 _y);

		void UpdateWindowSize(const s32 _width, const s32 _height);

		void RegisterCallbackForWMEvents(const u32 _wmEvent, const std::function<s32(HWND, u32, WPARAM, LPARAM)>& _func);

		std::vector<std::function<s32(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)>>& GetWMFunctions(u32 _wmEvent) const;

		HWND GetHwnd() const;

		SWindowDimParams GetCurrentDim() const;

	private:
		PMainWindow(const PWindowInfo& _windowInfo)
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

		PWindowInfo m_windowInfo;
		HWND m_wndHandle;
		static PMainWindow* s_mainWindow;
		std::unordered_map<u32, std::vector<std::function<s32(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)>>> m_wmEventMap;
	};
}