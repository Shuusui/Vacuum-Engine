#pragma once
#include "Renderer.h"

namespace Protostar::Core
{
	class Window;
}

namespace PSC = Protostar::Core;

namespace Protostar::Rendering
{
	class RENDERER_API IRenderer
	{
	public:
		IRenderer(PSC::Window*) {};
		virtual void Render() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
		virtual ~IRenderer();

		IRenderer(IRenderer&&) = delete;
		IRenderer(const IRenderer&) = delete;
		IRenderer& operator=(const IRenderer&) = delete;
	};
}