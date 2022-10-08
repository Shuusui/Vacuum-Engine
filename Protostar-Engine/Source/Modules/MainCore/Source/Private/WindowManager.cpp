#include "WindowManager.h"

namespace Protostar::Core
{
	bool WindowManager::Init(const WindowInfo& _windowInfo, std::string& _errorMsg)
	{
		if (s_windowManager)
		{
			_errorMsg = "Window manager got already created.";
			return false;
		}
		s_windowManager = new WindowManager();
		return s_windowManager->Create(_windowInfo, _errorMsg);
	}
	void WindowManager::Destroy()
	{
		delete s_windowManager;
	}
	bool WindowManager::Create(const WindowInfo& _windowInfo, std::string& _errorMsg)
	{
		m_mainWindow = WindowNode{ {}, std::unique_ptr<Window>(new Window(_windowInfo)), nullptr };
		if (!m_mainWindow.ThisWindow->Create(_errorMsg))
		{
			return false;
		}
		return true;
	}
}