#pragma once
#include "imgui.h"
#include <Windows.h>
#include <d3d12.h>
#include "GlobalDefs.h"

namespace Vacuum
{
	class CGUI
	{
	public:
		static bool Init(HWND _hwnd);
		static void NewFrame();
		static void Render();
		static void SetCaptureIfNotSet(HWND _hwnd);
		static void ReleaseCaptureIfSet(HWND _hwnd);
		static int32 OnLButtonDown(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);
		static int32 OnRButtonDown(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);
		static int32 OnLButtonUp(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);
		static int32 OnRButtonUp(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);
		static int32 OnSetCursor(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);
		static int32 OnKeyDown(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);
		static int32 OnKeyUp(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);
		static int32 OnChar(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);
		static int32 OnMouseWheel(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);
		static int32 OnMouseHWheel(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam);

		void UpdateMousePos();
		bool UpdateMouseCursor();

	private: 
		CGUI(HWND _hwnd)
			:m_hwnd(_hwnd)
			,m_lastMouseCursor(ImGuiMouseCursor_COUNT)
		{
		}

		static CGUI* s_gui;

		HWND m_hwnd;
		ImGuiMouseCursor m_lastMouseCursor;
	};
}