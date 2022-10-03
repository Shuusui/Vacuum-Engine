#include "App.h"
#include "WindowManager.h"

bool Protostar::Core::App::Init(HINSTANCE _hInstance, s32 _nCmdShow, std::string& _errorMsg)
{
	if (s_app)
	{
		return false;
	}
	s_app = new App();
	if (!WindowManager::Init
	(
		{
			{
				TEXT("Protostar Engine window"),
				_hInstance,
				CreateSolidBrush(RGB(1,1,1)),
				CS_HREDRAW | CS_VREDRAW
			},
			{
				1920,
				1080,
				0,
				0
			},
			{
				TEXT("Protostar-Engine"),
				0,
				WS_OVERLAPPEDWINDOW,
				nullptr,
				nullptr,
				nullptr
			}
		},
		_errorMsg)
	)
	{
		return false;
	}
	Window* mainWindow = WindowManager::Get().GetMainWindow();
	mainWindow->ShowWindow(_nCmdShow);
	mainWindow->UpdateWindow();
	return true;
}

void Protostar::Core::App::Destroy()
{
	if (s_app)
	{
		delete s_app;
	}
}

bool Protostar::Core::App::Update(MSG& msg) const
{
	m_timer->Update();
	if (Window* mainWindow = WindowManager::Get().GetMainWindow())
	{
		return mainWindow->Run(msg);
	}
	return false;
}

Protostar::Core::App::App()
	:m_timer(std::make_unique<Timer>())
{
}
