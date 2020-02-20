#pragma once
#include <Windows.h>
#include "GlobalDefs.h"
#include <string>

namespace Vacuum
{
	namespace Core
	{
		struct SWindowClassExInfo
		{
			uint32		m_style				=		CS_HREDRAW | CS_VREDRAW;
			WNDPROC		m_wndProc			=		nullptr;
			int32		m_cbClsExtra		=		NULL;
			int32		m_cbWndextra		=		NULL;
			HINSTANCE	m_hInstance			=		nullptr;
			HICON		m_hIcon				=		nullptr;
			HCURSOR		m_hCursor			=		nullptr;
			HBRUSH		m_hbrBackground		=		nullptr;
			LPCTSTR		m_lpszMenuName		=		nullptr;
			LPCTSTR		m_lpszClassName		=		nullptr;
			HICON		m_hIconSm			=		nullptr;
		};

		struct SWindowExInfo
		{
			DWORD		m_dwExStyle			=		WS_EX_OVERLAPPEDWINDOW;
			LPCTSTR		m_lpClassName		=		nullptr;
			LPCTSTR		m_lpWindowName		=		nullptr;
			DWORD		m_dwStyle			=		WS_OVERLAPPED | WS_MAXIMIZE | WS_BORDER;
			int32		m_X					=		NULL;
			int32		m_Y					=		NULL;
			int32		m_nWidth			=		NULL;
			int32		m_nHeight			=		NULL;
			HWND		m_hWndParent		=		nullptr;
			HMENU		m_hMenu				=		nullptr;
			HINSTANCE	m_hInstance			=		nullptr;	
			LPVOID		m_lpParam			=		nullptr;
		};

		class CWindow
		{
		public:
			static HWND ConstructWindow(const SWindowClassExInfo& wndClassInfo, const SWindowExInfo& wndInfo, std::string& errorMsg);

		};
	}
}