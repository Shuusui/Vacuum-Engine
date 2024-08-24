#include "InputProcessor.h"
#include "Window.h"
#include "Logger.h"

Protostar::Core::InputProcessor::InputProcessor(Window& _owningWindow)
	:m_owningWindow(_owningWindow)
	,m_mouseKeysBitMap(0)
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

	//Todo FixedSizeArray needs ranged based support
	//Todo This should be done like the m_mouseKeysBitMap
	for (size_t i = 0; i < m_keysBitMap.GetSize(); ++i)
	{
		m_keysBitMap[i] = 0;
	}
}

void Protostar::Core::InputProcessor::RegisterInputDownCallback(u8 _key, const std::function<void()>& _func)
{
	m_keyDownMap[_key].Add(_func);
}

void Protostar::Core::InputProcessor::RegisterInputUpCallback(u8 _key, const std::function<void()>& _func)
{
	m_keyUpMap[_key].Add(_func);
}

s32 Protostar::Core::InputProcessor::OnKeyDown(void*, u32, s64 _wParam, s64)
{
	m_keysBitMap[_wParam / 64] |= 1ULL << static_cast<u64>(_wParam - (64 * (_wParam / 64)));
	for (const std::function<void()>& func : m_keyDownMap[_wParam])
	{
		func();
	}
	return 0;
}

s32 Protostar::Core::InputProcessor::OnLButtonDown(void*, u32, s64 _wParam, s64)
{
	m_mouseKeysBitMap |= _wParam;
	return 0;
}

s32 Protostar::Core::InputProcessor::OnRButtonDown(void*, u32, s64 _wParam, s64)
{
	m_mouseKeysBitMap |= _wParam;
	return 0;
}

s32 Protostar::Core::InputProcessor::OnMButtonDown(void*, u32, s64 _wParam, s64)
{
	m_mouseKeysBitMap |= _wParam;
	return 0;
}

s32 Protostar::Core::InputProcessor::OnXButtonDown(void*, u32, s64 _wParam, s64)
{
	m_mouseKeysBitMap |= (_wParam & ~(MK_XBUTTON1) & ~(MK_XBUTTON2));
	return 0;
}

s32 Protostar::Core::InputProcessor::OnKeyUp(void*, u32, s64 _wParam, s64)
{
	m_keysBitMap[_wParam / 64] &= ~(1ULL << static_cast<u64>(_wParam - (64 * (_wParam / 64))));
	for (const std::function<void()>& func : m_keyUpMap[_wParam])
	{
		func();
	}
	return 0;
}

s32 Protostar::Core::InputProcessor::OnLButtonUp(void*, u32, s64, s64)
{
	m_mouseKeysBitMap &= ~(VK_LBUTTON);
	return 0;
}

s32 Protostar::Core::InputProcessor::OnRButtonUp(void*, u32, s64, s64)
{
	m_mouseKeysBitMap &= ~(VK_RBUTTON);
	return 0;
}

s32 Protostar::Core::InputProcessor::OnMButtonUp(void*, u32, s64 _wParam, s64)
{
	m_mouseKeysBitMap &= ~(_wParam);
	return 0;
}

s32 Protostar::Core::InputProcessor::OnXButtonUp(void*, u32, s64 _wParam, s64)
{
	m_mouseKeysBitMap &= ~(_wParam);
	return 0;
}