#pragma once

#include "SharedStructs.h"
#include <memory>
#include <IRenderer.h>

#ifdef RHI_LIBRARY
	#define RHI_API __declspec(dllexport)
#else
	#define RHI_API __declspec(dllimport)
#endif

namespace Protostar::Rendering
{
	class RHI_API RHI
	{
	public:
		static void Create(IRenderer* _renderer);
		static void Destroy();
		~RHI();
	private:
		static RHI* s_rhi;
		RHI(IRenderer* _renderer);
		std::unique_ptr<IRenderer> m_renderer;
	};
}
