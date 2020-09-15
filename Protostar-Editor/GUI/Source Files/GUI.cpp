#include "..\Header Files\GUI.h"
#include "Window.h"
#include "Timer.h"
#include "..\AppGUI\Header Files\AppMenuBar.h"
#include "..\ProjectGUI\Header Files\ProjectGUI.h"
#include "AppManager.h"
#include <fstream>
#include "Json.h"
#include "Log.h"
#include "RendererManager.h"

Protostar::CGUI* Protostar::CGUI::s_gui = nullptr;

bool Protostar::CGUI::Init(HWND _hwnd)
{
	if (s_gui)
	{
		return false;
	}

	s_gui = new CGUI(_hwnd);

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	io.BackendPlatformName = "Protostar-Editor";
	io.ImeWindowHandle = _hwnd;

	io.KeyMap[ImGuiKey_Tab] = VK_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Space] = VK_SPACE;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';

	
	CMainWindow* mainWindow = CMainWindow::GetWindowHandle();
	auto updateDisplaySize = [mainWindow](HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)->s32 {
		if (!mainWindow)
		{
			return 0;
		}

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(mainWindow->GetCurrentDim().Width);
		io.DisplaySize.y = static_cast<float>(mainWindow->GetCurrentDim().Height);
		return 0;
	};

	updateDisplaySize(nullptr, 0, 0, 0);

	mainWindow->RegisterCallbackForWMEvents(WM_EXITSIZEMOVE, updateDisplaySize);
	mainWindow->RegisterCallbackForWMEvents(WM_LBUTTONDOWN, &Protostar::CGUI::OnLButtonDown);
	mainWindow->RegisterCallbackForWMEvents(WM_RBUTTONDOWN, &Protostar::CGUI::OnRButtonDown);
	mainWindow->RegisterCallbackForWMEvents(WM_LBUTTONUP, &Protostar::CGUI::OnLButtonUp);
	mainWindow->RegisterCallbackForWMEvents(WM_RBUTTONUP, &Protostar::CGUI::OnRButtonUp);
	mainWindow->RegisterCallbackForWMEvents(WM_SETCURSOR, &Protostar::CGUI::OnSetCursor);
	mainWindow->RegisterCallbackForWMEvents(WM_KEYDOWN, &Protostar::CGUI::OnKeyDown);
	mainWindow->RegisterCallbackForWMEvents(WM_KEYUP, &Protostar::CGUI::OnKeyUp);
	mainWindow->RegisterCallbackForWMEvents(WM_CHAR, &Protostar::CGUI::OnChar);
	mainWindow->RegisterCallbackForWMEvents(WM_MOUSEWHEEL, &Protostar::CGUI::OnMouseWheel);
	mainWindow->RegisterCallbackForWMEvents(WM_MOUSEHWHEEL, &Protostar::CGUI::OnMouseHWheel);

	CRendererManager::RegisterAfterResizeCallback(&Protostar::CGUI::OnUpdateFontTexture);

	s_gui->LoadGUIIniFile();

	s_gui->m_appMenuBar = new CAppMenuBar();
	CProjectGUI::OnCreate();

	return true;
}

void Protostar::CGUI::NewFrame()
{

	ImGuiIO& io = ImGui::GetIO();

	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	io.DeltaTime = CTimer::GetDeltaSeconds();

	io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
	io.KeySuper = false;

	s_gui->UpdateMousePos();

	ImGuiMouseCursor mouseCursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
	if (s_gui->m_lastMouseCursor != mouseCursor)
	{
		s_gui->m_lastMouseCursor = mouseCursor;
		s_gui->UpdateMouseCursor();
	}

	ImGui::NewFrame();

	s_gui->RenderGUIElements();
}

void Protostar::CGUI::Render()
{

	ImGui::Render();

	ImDrawData* guiDrawData = ImGui::GetDrawData();

	DirectX::XMFLOAT2 displayPos = {guiDrawData->DisplayPos.x, guiDrawData->DisplayPos.y};
	DirectX::XMFLOAT2 displaySize = {guiDrawData->DisplaySize.x, guiDrawData->DisplaySize.y};
	SGuiDrawData* drawData = new SGuiDrawData{displayPos, displaySize, guiDrawData->TotalIdxCount, guiDrawData->TotalVtxCount};

	for (s32 i = 0; i < guiDrawData->CmdListsCount; ++i)
	{
		ImDrawList* guiDrawList = guiDrawData->CmdLists[i];
		SGuiDrawList drawList = {};
		drawList.IndexBuffer = std::vector<unsigned short>(guiDrawList->IdxBuffer.Data, guiDrawList->IdxBuffer.Data + guiDrawList->IdxBuffer.Size);
		drawList.VertexBuffer = std::vector<S2DVert>((S2DVert*)guiDrawList->VtxBuffer.Data, (S2DVert*)guiDrawList->VtxBuffer.Data + guiDrawList->VtxBuffer.Size);

		for (s32 j = 0; j < guiDrawList->CmdBuffer.Size; ++j)
		{
			ImDrawCmd* guiDrawCmd = &guiDrawList->CmdBuffer[j];

			SGuiDrawCmd drawCmd = {};
			if (guiDrawCmd->UserCallback != nullptr)
			{
				drawCmd.UserCallbackValid = true;
			}
			if (guiDrawCmd->UserCallback != ImDrawCallback_ResetRenderState)
			{
				drawCmd.UserCallback = std::function<void()>([guiDrawList, guiDrawCmd]()->void
				{
					guiDrawCmd->UserCallback(guiDrawList, guiDrawCmd);
				});
				drawCmd.CallUserCallback = true;
			}
			drawCmd.ClipRect = {guiDrawCmd->ClipRect.x, guiDrawCmd->ClipRect.y, guiDrawCmd->ClipRect.z, guiDrawCmd->ClipRect.w};
			drawCmd.ElemCount = guiDrawCmd->ElemCount;
			drawCmd.IdxOffset = guiDrawCmd->IdxOffset;
			drawCmd.TextureID = guiDrawCmd->TextureId;
			drawCmd.VtxOffset = guiDrawCmd->VtxOffset;
			drawList.DrawCommands.push_back(drawCmd);
		}
		drawData->DrawLists.push_back(drawList);
	}

	CRendererManager::UpdateGuiDrawData(drawData);
}

void Protostar::CGUI::SetCaptureIfNotSet(HWND _hwnd)
{
	if (!ImGui::IsAnyMouseDown() && GetCapture() == nullptr)
	{
		SetCapture(_hwnd);
	}
}

void Protostar::CGUI::ReleaseCaptureIfSet(HWND _hwnd)
{
	if (!ImGui::IsAnyMouseDown() && GetCapture() == _hwnd)
	{
		ReleaseCapture();
	}
}

s32 Protostar::CGUI::OnLButtonDown(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	SetCaptureIfNotSet(_hwnd);
	io.MouseDown[0] = true;
	return 0;
}

s32 Protostar::CGUI::OnRButtonDown(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	SetCaptureIfNotSet(_hwnd);
	io.MouseDown[1] = true;
	return 0;
}

s32 Protostar::CGUI::OnLButtonUp(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = false;
	ReleaseCaptureIfSet(_hwnd);
	return 0;
}

s32 Protostar::CGUI::OnRButtonUp(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[1] = false;
	ReleaseCaptureIfSet(_hwnd);
	return 0;
}

s32 Protostar::CGUI::OnSetCursor(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (LOWORD(_lParam) == HTCLIENT && s_gui->UpdateMouseCursor())
	{
		return 1;
	}
	return 0;
}

s32 Protostar::CGUI::OnKeyDown(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (_wParam < 256)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[_wParam] = 1;
	}
	return 0;
}

s32 Protostar::CGUI::OnKeyUp(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (_wParam < 256)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[_wParam] = 0;
	}
	return 0;
}

s32 Protostar::CGUI::OnChar(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (_wParam > 0 && _wParam < 0x10000)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacterUTF16((u16)_wParam);
	}
	return 0;
}

s32 Protostar::CGUI::OnMouseWheel(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(_wParam) / (float)WHEEL_DELTA;
	return 0;
}

s32 Protostar::CGUI::OnMouseHWheel(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(_wParam) / (float)WHEEL_DELTA;
	return 0;
}

void Protostar::CGUI::OnUpdateFontTexture(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->TexID = nullptr;

	unsigned char* pixels;
	s32 width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	u64 texID = 0;
	CRendererManager::CreateFontsTexture(pixels, width, height, texID);
	io.Fonts->TexID = (void*)texID;
}

void Protostar::CGUI::Destroy()
{
	ImGui::DestroyContext();
	if (s_gui)
	{
		delete s_gui;
		s_gui = nullptr;
	}
}

Protostar::SGUIInfo& Protostar::CGUI::GetGUIInfo()
{
	return s_gui->m_guiInfo;
}

Protostar::CGUI::~CGUI()
{
	if (!std::filesystem::exists(m_guiIniPath))
	{
		std::filesystem::create_directories(m_guiIniPath);
	}

	std::ofstream appGuiIniFile(m_guiIniFilePath, std::ios::trunc);
	Json json;
	json["open_console"] = m_guiInfo.bOpenConsole;
	json["open_content_browser"] = m_guiInfo.bOpenContentBrowser;
	json["open_editor_fps"] = m_guiInfo.bOpenEditorFPS;
	appGuiIniFile << json.dump();
}

void Protostar::CGUI::UpdateMousePos()
{
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantSetMousePos)
	{
		POINT pos = {(s32)io.MousePos.x, (s32)io.MousePos.y};
		ClientToScreen(m_hwnd, &pos);
		SetCursorPos(pos.x, pos.y);
	}

	io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	POINT pos; 
	if (HWND activeWindow = ::GetForegroundWindow())
	{
		if (activeWindow == m_hwnd || IsChild(activeWindow, m_hwnd))
		{
			if (GetCursorPos(&pos) && ScreenToClient(m_hwnd, &pos))
			{
				io.MousePos = ImVec2((float)pos.x, (float)pos.y);
			}
		}
	}
}

bool Protostar::CGUI::UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
	{
		return false;
	}

	ImGuiMouseCursor imGuiCursor = ImGui::GetMouseCursor();
	if (imGuiCursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
	{
		SetCursor(nullptr);
	}
	else
	{
		LPTSTR win32Cursor = IDC_ARROW;
		switch (imGuiCursor)
		{
		case ImGuiMouseCursor_Arrow:        win32Cursor = IDC_ARROW; break;
		case ImGuiMouseCursor_TextInput:    win32Cursor = IDC_IBEAM; break;
		case ImGuiMouseCursor_ResizeAll:    win32Cursor = IDC_SIZEALL; break;
		case ImGuiMouseCursor_ResizeEW:     win32Cursor = IDC_SIZEWE; break;
		case ImGuiMouseCursor_ResizeNS:     win32Cursor = IDC_SIZENS; break;
		case ImGuiMouseCursor_ResizeNESW:   win32Cursor = IDC_SIZENESW; break;
		case ImGuiMouseCursor_ResizeNWSE:   win32Cursor = IDC_SIZENWSE; break;
		case ImGuiMouseCursor_Hand:         win32Cursor = IDC_HAND; break;
		case ImGuiMouseCursor_NotAllowed:   win32Cursor = IDC_NO; break;
		}
		SetCursor(LoadCursor(nullptr, win32Cursor));
	}
	return true;
}

void Protostar::CGUI::RenderGUIElements()
{
	m_appMenuBar->OnRender();
	CProjectGUI::GetProjectGUIHandle()->OnRender();
}

void Protostar::CGUI::LoadGUIIniFile()
{
	SAppPaths appPaths = CAppManager::GetAppPaths();
	m_guiIniPath =  appPaths.ConfigDir / "GUI";
	m_guiIniFilePath = m_guiIniPath / "appgui.ini";
	if (!std::filesystem::exists(m_guiIniFilePath))
	{
		return;
	}

	std::ifstream appGuiIniFile(m_guiIniFilePath);
	Json json;
	appGuiIniFile >> json;
	m_guiInfo.bOpenConsole = json["open_console"].get<bool>();
	m_guiInfo.bOpenContentBrowser = json["open_content_browser"].get<bool>();
	m_guiInfo.bOpenEditorFPS = json["open_editor_fps"].get<bool>();
}

void Protostar::CGUI::CreateAppMenuBar()
{
	if (m_appMenuBar)
	{
		return;
	}
	m_appMenuBar = new CAppMenuBar();
}

void Protostar::CGUI::DestroyAppMenuBar()
{
	if (m_appMenuBar)
	{
		delete m_appMenuBar;
		m_appMenuBar = nullptr;
	}
}

void Protostar::CGUI::CreateProjectGUI()
{
	CProjectGUI::OnCreate();
}

void Protostar::CGUI::DestroyProjectGUI()
{
	CProjectGUI::OnDestroy();
}
