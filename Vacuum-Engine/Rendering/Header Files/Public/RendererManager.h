#pragma once
#include "..\Private\IRenderer.h"
#include "..\Private\DX12Renderer.h"
#include "SharedStructs.h"
#include <filesystem>
#include <vector>

namespace Vacuum
{
	class CRendererManager
	{
	public:
		static void Create(const SRendererCreationInfo& _info);
		static void OnInit(const std::vector<std::filesystem::path>& _shaderPaths);
		static void OnRender();
		static void Destroy();
		
	private:
		CRendererManager(const SRendererCreationInfo& _info)
			:m_renderer(nullptr)
		{
			switch (_info.m_renderApi)
			{
			case ERenderAPIs::DX12:
				m_renderer = new DX12Renderer(_info.m_width, _info.m_height, (HWND)_info.m_wndHandle);
				break;
			}
		}
		static CRendererManager* s_rendererManager;
		IRenderer* m_renderer;
	};
}