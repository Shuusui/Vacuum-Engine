#include "..\Header Files\GUI.h"
#include "Window.h"
#include "Timer.h"
#include "..\Header Files\AppGUI\AppMenuBar.h"

Vacuum::CGUI* Vacuum::CGUI::s_gui = nullptr;

bool Vacuum::CGUI::Init(HWND _hwnd)
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

	io.BackendPlatformName = "Vacuum-Editor";
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
	auto updateDisplaySize = [mainWindow](HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)->int32 {
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
	mainWindow->RegisterCallbackForWMEvents(WM_LBUTTONDOWN, &Vacuum::CGUI::OnLButtonDown);
	mainWindow->RegisterCallbackForWMEvents(WM_RBUTTONDOWN, &Vacuum::CGUI::OnRButtonDown);
	mainWindow->RegisterCallbackForWMEvents(WM_LBUTTONUP, &Vacuum::CGUI::OnLButtonUp);
	mainWindow->RegisterCallbackForWMEvents(WM_RBUTTONUP, &Vacuum::CGUI::OnRButtonUp);
	mainWindow->RegisterCallbackForWMEvents(WM_SETCURSOR, &Vacuum::CGUI::OnSetCursor);
	mainWindow->RegisterCallbackForWMEvents(WM_KEYDOWN, &Vacuum::CGUI::OnKeyDown);
	mainWindow->RegisterCallbackForWMEvents(WM_KEYUP, &Vacuum::CGUI::OnKeyUp);
	mainWindow->RegisterCallbackForWMEvents(WM_CHAR, &Vacuum::CGUI::OnChar);
	mainWindow->RegisterCallbackForWMEvents(WM_MOUSEWHEEL, &Vacuum::CGUI::OnMouseWheel);
	mainWindow->RegisterCallbackForWMEvents(WM_MOUSEHWHEEL, &Vacuum::CGUI::OnMouseHWheel);

	return true;
}

void Vacuum::CGUI::NewFrame()
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

void Vacuum::CGUI::Render()
{
	ImGui::Render();
}

void Vacuum::CGUI::SetCaptureIfNotSet(HWND _hwnd)
{
	if (!ImGui::IsAnyMouseDown() && GetCapture() == nullptr)
	{
		SetCapture(_hwnd);
	}
}

void Vacuum::CGUI::ReleaseCaptureIfSet(HWND _hwnd)
{
	if (!ImGui::IsAnyMouseDown() && GetCapture() == _hwnd)
	{
		ReleaseCapture();
	}
}

int32 Vacuum::CGUI::OnLButtonDown(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	SetCaptureIfNotSet(_hwnd);
	io.MouseDown[0] = true;
	return 0;
}

int32 Vacuum::CGUI::OnRButtonDown(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	SetCaptureIfNotSet(_hwnd);
	io.MouseDown[1] = true;
	return 0;
}

int32 Vacuum::CGUI::OnLButtonUp(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = false;
	ReleaseCaptureIfSet(_hwnd);
	return 0;
}

int32 Vacuum::CGUI::OnRButtonUp(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[1] = false;
	ReleaseCaptureIfSet(_hwnd);
	return 0;
}

int32 Vacuum::CGUI::OnSetCursor(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (LOWORD(_lParam) == HTCLIENT && s_gui->UpdateMouseCursor())
	{
		return 1;
	}
	return 0;
}

int32 Vacuum::CGUI::OnKeyDown(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (_wParam < 256)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[_wParam] = 1;
	}
	return 0;
}

int32 Vacuum::CGUI::OnKeyUp(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (_wParam < 256)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[_wParam] = 1;
	}
	return 0;
}

int32 Vacuum::CGUI::OnChar(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	if (_wParam > 0 && _wParam < 0x10000)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacterUTF16((uint16)_wParam);
	}
	return 0;
}

int32 Vacuum::CGUI::OnMouseWheel(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(_wParam) / (float)WHEEL_DELTA;
	return 0;
}

int32 Vacuum::CGUI::OnMouseHWheel(HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(_wParam) / (float)WHEEL_DELTA;
	return 0;
}

void Vacuum::CGUI::Destroy()
{
	ImGui::DestroyContext();
	if (s_gui)
	{
		delete s_gui;
		s_gui = nullptr;
	}
}

void Vacuum::CGUI::UpdateMousePos()
{
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantSetMousePos)
	{
		POINT pos = {(int32)io.MousePos.x, (int32)io.MousePos.y};
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

bool Vacuum::CGUI::UpdateMouseCursor()
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

void Vacuum::CGUI::RenderGUIElements()
{
	m_appMenuBar->OnRender();
}

void Vacuum::CGUI::CreateAppMenuBar()
{
	if (m_appMenuBar)
	{
		return;
	}
	m_appMenuBar = new CAppMenuBar();
}

void Vacuum::CGUI::DestroyAppMenuBar()
{
	if (m_appMenuBar)
	{
		delete m_appMenuBar;
		m_appMenuBar = nullptr;
	}
}
