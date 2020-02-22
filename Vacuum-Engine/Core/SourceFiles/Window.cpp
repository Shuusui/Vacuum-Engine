#include "../HeaderFiles/Window.h"
#include "../HeaderFiles/Util.h"

namespace Vacuum
{
	namespace Core
	{
		HWND CWindow::ConstructWindow(const SWindowClassExInfo& wndClassInfo, const SWindowExInfo& wndInfo, std::wstring& errorMsg)
		{
			WNDCLASSEX wndClass = WNDCLASSEX{
				sizeof(WNDCLASSEX),				
				wndClassInfo.m_style,			
				wndClassInfo.m_wndProc,			
				wndClassInfo.m_cbClsExtra,		
				wndClassInfo.m_cbWndextra,		
				wndClassInfo.m_hInstance,		
				wndClassInfo.m_hIcon,			
				wndClassInfo.m_hCursor,			
				wndClassInfo.m_hbrBackground,	
				wndClassInfo.m_lpszMenuName,	
				wndClassInfo.m_lpszClassName,	
				wndClassInfo.m_hIconSm,			
			};

			ATOM atom = RegisterClassEx(&wndClass);
			if (!atom)
			{
				errorMsg = GetLastErrorString();
				return nullptr;
			}

			HWND returnHandle = CreateWindowEx(
				wndInfo.m_dwExStyle,			
				wndInfo.m_lpClassName,			
				wndInfo.m_lpWindowName, 
				wndInfo.m_dwStyle,
				wndInfo.m_X,
				wndInfo.m_Y,
				wndInfo.m_nWidth,
				wndInfo.m_nHeight, 
				wndInfo.m_hWndParent,
				wndInfo.m_hMenu,
				wndInfo.m_hInstance,
				wndInfo.m_lpParam
			);

			if (!returnHandle)
			{
				errorMsg = GetLastErrorString();
				return nullptr;
			}
			return returnHandle;
		}
	}
}