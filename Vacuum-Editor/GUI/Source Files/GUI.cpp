#include "..\Header Files\GUI.h"
#include "Window.h"
#include "Timer.h"

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
	auto lambda = [mainWindow]()->void {
		if (!mainWindow)
		{
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(mainWindow->GetCurrentDim().m_width);
		io.DisplaySize.y = static_cast<float>(mainWindow->GetCurrentDim().m_height);
	};

	lambda();

	mainWindow->RegisterCallbackforWMEvents(WM_EXITSIZEMOVE, lambda);

	return true;
}

void Vacuum::CGUI::NewFrame()
{
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();

	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	io.DeltaTime = CTimer::GetDeltaSeconds();

	io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
	io.KeySuper = false;

	ImGui::Render();
}

void Vacuum::CGUI::UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
	{
		return;
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
}
