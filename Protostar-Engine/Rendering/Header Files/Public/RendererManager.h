#pragma once
#include "..\Private\IRenderer.h"
#include "..\Private\DX12Renderer.h"
#include "SharedStructs.h"
#include <filesystem>
#include <vector>
#include <d3d12.h>
#include <functional>

namespace Protostar
{
	class CRendererManager
	{
	public:
		static void Create(const SRendererCreationInfo& _info);
		static void CreateFontsTexture(unsigned char* _pixels, const s32& _width, const s32& _height, u64& _texID);
		static void PrepareRendering();
		static void UpdateDrawData(struct SGuiDrawData* _drawData);
		static void UpdateVPDrawData(struct SDrawData* _drawData);
		static void OnInit(const std::vector<std::filesystem::path>& _shaderPaths);
		static void OnUpdate();
		static void OnRender();
		static void Destroy();
		static void RegisterAfterResizeCallback(const std::function<void(HWND, u32, WPARAM, LPARAM)>& _func);
		static bool& GetVSync()
		{
			return s_rendererManager->m_bVSync;
		}
		
	private:
		CRendererManager(const SRendererCreationInfo& _info)
			:m_renderer(nullptr)
			,m_bVSync(_info.bVSync)
		{
			switch (_info.RenderApi)
			{
			case ERenderAPIs::DX12:
				m_renderer = new DX12Renderer(_info.Width, _info.Height, (HWND)_info.WndHandle, m_bVSync);
				m_renderer->OnCreate();
				break;
			}
		}
		static CRendererManager* s_rendererManager;
		IRenderer* m_renderer;
		bool m_bVSync;
	};
}