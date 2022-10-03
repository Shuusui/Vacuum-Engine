#pragma once

#include "SharedStructs.h"
#include <memory>
#include <IRenderer.h>


namespace Protostar::Rendering
{
	class RHI
	{
	public:
		static void Create(IRenderer* _renderer);
		static void Destroy();
	private:
		static RHI* s_rhi;
		RHI(IRenderer* _renderer);
		std::unique_ptr<IRenderer> m_renderer;
	};
}
