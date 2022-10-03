#include "RHI.h"

Protostar::Rendering::RHI* Protostar::Rendering::RHI::s_rhi = nullptr;

namespace Protostar::Rendering
{

	void RHI::Create(IRenderer* _renderer)
	{
		if (!s_rhi)
		{
			s_rhi = new RHI(_renderer);
		}
	}

	void RHI::Destroy()
	{

	}

	RHI::RHI(IRenderer* _renderer)
		:m_renderer(_renderer)
	{
	}
}
