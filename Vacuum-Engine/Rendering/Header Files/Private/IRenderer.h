#pragma once
#include "GlobalDefs.h"
#include <filesystem>
#include <vector>
#include <d3d12.h>
#include <functional>

namespace Vacuum
{
	class IRenderer
	{
	public:
		IRenderer() = delete;

		IRenderer(const uint32& _width, const uint32& _height, void* _wndHandle, bool& _bVSync)
			:m_width(_width)
			,m_height(_height)
			,m_wndHandle(_wndHandle)
			,m_bVSync(_bVSync)
		{
		}

		virtual ~IRenderer() = default;

		virtual void OnCreate() = 0;
		virtual void CreateFontsTexture(unsigned char* _pixels, const int32& _width, const int32& _height, uint64& _texID) = 0;
		virtual void UpdateDrawData(struct SGuiDrawData* _drawData) = 0;
		virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
		virtual void PrepareRendering() = 0;
		virtual void OnRender() = 0;
		virtual void OnDestroy() = 0;
		virtual void RegisterAfterResizeCallback(const std::function<void(HWND, uint32, WPARAM, LPARAM)>& _func) = 0;

		void SetShaderPaths(const std::vector<std::filesystem::path>& _shaderPaths)
		{
			m_shaderPaths = _shaderPaths;
		}
	protected:
		uint32 m_width;
		uint32 m_height;
		void* m_wndHandle;
		std::vector<std::filesystem::path> m_shaderPaths;
		bool& m_bVSync;
	};
}