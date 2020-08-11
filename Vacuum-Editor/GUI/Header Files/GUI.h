#pragma once
#include "imgui.h"
#include <Windows.h>
#include <d3d12.h>

namespace Vacuum
{
	class CGUI
	{
	public:
		static bool Init(HWND _hwnd);
		static void NewFrame();
		void UpdateMouseCursor();

	private: 
		CGUI(HWND _hwnd)
			:m_hwnd(_hwnd)
		{
		}

		static CGUI* s_gui;

		HWND m_hwnd;
	};
}