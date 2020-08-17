#pragma once
#include "..\Private\IRenderer.h"
#include "..\Private\DX12Renderer.h"
#include "SharedStructs.h"
#include <filesystem>
#include <vector>
#include <d3d12.h>

namespace Vacuum
{
	class CRendererManager
	{
	public:
		static void Create(const SRendererCreationInfo& _info);
		static void OnInit(const std::vector<std::filesystem::path>& _shaderPaths);
		static void PrepareRendering();
		static void OnRender();
		static void Destroy();
		
	private:
		CRendererManager(const SRendererCreationInfo& _info)
			:m_renderer(nullptr)
		{
			switch (_info.RenderApi)
			{
			case ERenderAPIs::DX12:
				m_renderer = new DX12Renderer(_info.Width, _info.Height, (HWND)_info.WndHandle);
				m_renderer->OnCreate();
				break;
			}
		}
		static CRendererManager* s_rendererManager;
		IRenderer* m_renderer;
	};
}