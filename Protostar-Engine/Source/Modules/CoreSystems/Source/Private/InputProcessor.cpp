#include "InputProcessor.h"
#include "Window.h"
#include "Logger.h"

Protostar::Core::InputProcessor::InputProcessor(Window& _owningWindow)
	:m_owningWindow(_owningWindow)
{
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_SYSKEYDOWN, this, &InputProcessor::OnKeyDown);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_KEYDOWN, this, &InputProcessor::OnKeyDown);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_LBUTTONDOWN, this, &InputProcessor::OnLButtonDown);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_RBUTTONDOWN, this, &InputProcessor::OnRButtonDown);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_MBUTTONDOWN, this, &InputProcessor::OnMButtonDown);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_XBUTTONDOWN, this, &InputProcessor::OnXButtonDown);

	_owningWindow.REGISTER_EVENT_CALLBACK(WM_SYSKEYUP, this, &InputProcessor::OnKeyUp);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_KEYUP, this, &InputProcessor::OnKeyUp);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_LBUTTONUP, this, &InputProcessor::OnLButtonUp);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_RBUTTONUP, this, &InputProcessor::OnRButtonUp);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_MBUTTONDOWN, this, &InputProcessor::OnMButtonUp);
	_owningWindow.REGISTER_EVENT_CALLBACK(WM_XBUTTONUP, this, &InputProcessor::OnXButtonUp);

	_owningWindow.REGISTER_EVENT_CALLBACK(WM_MOUSEWHEEL, this, &InputProcessor::OnMouseWheel);
}

//Todo: remove
#pragma warning(disable : 4100)
s32 Protostar::Core::InputProcessor::OnKeyDown(void*, u32, s64 _wParam, s64)
{

	return 0;
}

s32 Protostar::Core::InputProcessor::OnLButtonDown(void*, u32, s64 _wParam, s64)
{
	return 0;
}

s32 Protostar::Core::InputProcessor::OnRButtonDown(void*, u32, s64 _wParam, s64)
{
	return 0;
}

s32 Protostar::Core::InputProcessor::OnMButtonDown(void*, u32, s64 _wParam, s64)
{
	return 0;
}

s32 Protostar::Core::InputProcessor::OnXButtonDown(void*, u32, s64 _wParam, s64)
{
	return 0;
}

s32 Protostar::Core::InputProcessor::OnKeyUp(void*, u32, s64 _wParam, s64)
{
	return 0;
}

s32 Protostar::Core::InputProcessor::OnLButtonUp(void*, u32, s64 _wParam, s64)
{
	return 0;
}

s32 Protostar::Core::InputProcessor::OnRButtonUp(void*, u32, s64 _wParam, s64)
{
	return 0;
}

s32 Protostar::Core::InputProcessor::OnMButtonUp(void*, u32, s64 _wParam, s64)
{
	return 0;
}

s32 Protostar::Core::InputProcessor::OnXButtonUp(void*, u32, s64 _wParam, s64)
{
	return 0;
}

s32 Protostar::Core::InputProcessor::OnMouseWheel(void*, u32, s64 _wParam, s64)
{
	return 0;
}

#pragma warning(default : 4100)