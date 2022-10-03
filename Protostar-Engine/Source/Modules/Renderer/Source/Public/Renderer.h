#pragma once

#ifdef RENDERER_LIBRARY
	#define RENDERER_API __declspec(dllexport)
#else
	#define RENDERER_API __declspec(dllimport)
#endif

namespace Protostar::Rendering
{
	class RENDERER_API RendererModule
	{
	public:
		void StartupModule();
		void ShutdownModule();
	};
}