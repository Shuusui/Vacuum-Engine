#pragma once
#include "GlobalDefs.h"

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

		virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnDestroy() = 0;
	protected:
		uint32 m_width;
		uint32 m_height;
		void* m_wndHandle;
	};
}