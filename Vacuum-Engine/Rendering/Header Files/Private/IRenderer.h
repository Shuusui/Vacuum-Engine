#pragma once
#include "GlobalDefs.h"
#include <filesystem>
#include <vector>
#include <d3d12.h>

namespace Vacuum
{
	class IRenderer
	{
	public:
		IRenderer() = delete;

		IRenderer(const uint32& _width, const uint32& _height, void* _wndHandle)
			:m_width(_width)
			,m_height(_height)
			,m_wndHandle(_wndHandle)
		{
		}

		virtual ~IRenderer() = default;

		virtual void OnCreate() = 0;
		virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
		virtual void PrepareRendering() = 0;
		virtual void OnRender() = 0;
		virtual void OnDestroy() = 0;
		void SetShaderPaths(const std::vector<std::filesystem::path>& _shaderPaths)
		{
			m_shaderPaths = _shaderPaths;
		}
	protected:
		uint32 m_width;
		uint32 m_height;
		void* m_wndHandle;
		std::vector<std::filesystem::path> m_shaderPaths;
	};
}