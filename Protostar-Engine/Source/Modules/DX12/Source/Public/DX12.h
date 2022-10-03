#pragma once
#include "IRenderer.h"

#ifdef DX12_LIBRARY
	#define DX12_API __declspec(dllexport)
#else
	#define DX12_API __declspec(dllimport)
#endif

namespace Protostar::Rendering
{
	class DX12_API DX12 : public IRenderer
	{
	public:
		DX12() = default;
		virtual void Render() override;
		virtual void Update() override;
		virtual ~DX12() override = default;
	};
}