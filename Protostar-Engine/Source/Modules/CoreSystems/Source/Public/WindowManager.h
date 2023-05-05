#pragma once
#include "MainCore.h"
#include "Window.h"
#include <memory>
#include <cstring>
#include <vector>
#include "CoreSystems.h"

namespace Protostar::Core
{
	class CORESYSTEMS_API WindowManager
	{
	public:
		static bool Init(const WindowInfo& _windowInfo, std::string& _errorMsg);
		static const WindowManager& Get()
		{
			return *s_windowManager;
		}
		static void Destroy();
		Window* GetMainWindow() const
		{
			return m_mainWindow.ThisWindow.Get();
		}
	private:
		struct WindowNode
		{
			std::vector<Window*> ChildWindows;
			UniquePtr<Window> ThisWindow = nullptr;
			Window* Parent = nullptr;
		};
		bool Create(const WindowInfo& _windowInfo, std::string& _errorMsg);
		static inline WindowManager* s_windowManager = nullptr;
		WindowNode m_mainWindow;
	};
}