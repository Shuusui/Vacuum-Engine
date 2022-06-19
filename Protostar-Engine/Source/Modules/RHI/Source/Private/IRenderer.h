#ifndef IRENDERER_H
#define IRENDERER_H

#include "GlobalDefinitions.h"
#include <filesystem>
#include <vector>
#include <d3d12.h>
#include <functional>

namespace Protostar
{
	class IRenderer
	{
	public:
		IRenderer() = delete;

		IRenderer(const u32 _width, const u32 _height, void* _wndHandle, bool& _bVSync)
			:m_width(_width)
			,m_height(_height)
			,m_wndHandle(_wndHandle)
			,m_bVSync(_bVSync)
		{
		}

		virtual ~IRenderer() = default;

		virtual void OnCreate() = 0;
		virtual void CreateFontsTexture(unsigned char* _pixels, const s32 _width, const s32 _height, u64& _texID) = 0;
		virtual void UpdateGuiDrawData(struct SGuiDrawData* _drawData) = 0;
		virtual void AddDrawData(struct SDrawData* _drawData) = 0;
		virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
		virtual void PrepareRendering() = 0;
		virtual void OnRender() = 0;
		virtual void OnDestroy() = 0;
		virtual void RegisterAfterResizeCallback(const std::function<void(HWND, u32, WPARAM, LPARAM)>& _func) = 0;
		virtual bool CreateRootSignature(ID3DBlob* _rootSignatureBlob, ID3D12RootSignature** _rootSignature) = 0;
		virtual bool CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, ID3D12PipelineState** _pso) = 0;

		void SetShaderPaths(const std::vector<std::filesystem::path>& _shaderPaths)
		{
			m_shaderPaths = _shaderPaths;
		}
	protected:
		u32 m_width;
		u32 m_height;
		void* m_wndHandle;
		std::vector<std::filesystem::path> m_shaderPaths;
		bool& m_bVSync;
	};
}
#endif //IRENDERER_H