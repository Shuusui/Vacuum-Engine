#pragma once
#include "imgui.h"
#include <Windows.h>
#include <d3d12.h>
#include "GlobalDefs.h"
#include <filesystem>
#include "RendererManager.h"

namespace Vacuum
{
	struct SGUIInfo
	{
		bool bOpenConsole;
		bool bOpenContentBrowser;
		bool bOpenEditorFPS;
	};

	class CGUI
	{
	public:
		static bool Init(HWND _hwnd);
		static void NewFrame();
		static void Render();
		static void Destroy();
		static SGUIInfo& GetGUIInfo();

		static void SetOpenLog(const bool& _bConsoleOpen)
		{
			s_gui->m_guiInfo.bOpenConsole = _bConsoleOpen;
		}

		static void SetOpenContentBrowser(const bool& _bContentBrowserOpen)
		{
			s_gui->m_guiInfo.bOpenContentBrowser = _bContentBrowserOpen;
		}

		static void SetOpenEditorFPS(const bool& _bOpenEditorFPS)
		{
			s_gui->m_guiInfo.bOpenEditorFPS = _bOpenEditorFPS;
		}

		static void SetVPDrawData(SDrawData* _drawData)
		{
			s_gui->m_vpDrawData = _drawData;
		}

		~CGUI();

		void CreateAppMenuBar();
		void DestroyAppMenuBar();

		void CreateProjectGUI();
		void DestroyProjectGUI();

		void UpdateMousePos();
		bool UpdateMouseCursor();

		void RenderGUIElements();

		void LoadGUIIniFile();

	private: 
		static void SetCaptureIfNotSet(HWND _hwnd);
		static void ReleaseCaptureIfSet(HWND _hwnd);
		static s32 OnLButtonDown(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static s32 OnRButtonDown(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static s32 OnLButtonUp(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static s32 OnRButtonUp(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static s32 OnSetCursor(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static s32 OnKeyDown(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static s32 OnKeyUp(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static s32 OnChar(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static s32 OnMouseWheel(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static s32 OnMouseHWheel(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);
		static void OnUpdateFontTexture(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);

		CGUI(HWND _hwnd)
			:m_hwnd(_hwnd)
			,m_lastMouseCursor(ImGuiMouseCursor_COUNT)
			,m_appMenuBar(nullptr)
			,m_guiInfo({})
		{

		}

		static CGUI* s_gui;

		HWND m_hwnd;
		ImGuiMouseCursor m_lastMouseCursor;
		class CAppMenuBar* m_appMenuBar;
		std::filesystem::path m_guiIniFilePath;
		std::filesystem::path m_guiIniPath;
		SGUIInfo m_guiInfo;
		SDrawData* m_vpDrawData;
	};
}