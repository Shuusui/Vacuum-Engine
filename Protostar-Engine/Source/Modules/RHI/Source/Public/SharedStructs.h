#pragma once

#include <utility>
#include <vector>
#include <algorithm>

#include "GlobalDefinitions.h"
#include "SharedEnums.h"
#include "DirectXMath.h"
#include "d3d12.h"
#include "Json.h"
#include "Guid.h"

namespace Protostar::Core
{
	/*struct PPSOInfo
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDescs;
		std::pair<Guid, ID3D12RootSignature*> RootSignature;
		std::pair<Guid, ID3DBlob*> VertexShader;
		std::pair<Guid, ID3DBlob*> PixelShader;
		u32 NodeMask;
		D3D12_BLEND_DESC BlendDesc;
		D3D12_RASTERIZER_DESC RasterizerDesc;
		D3D12_DEPTH_STENCIL_DESC DepthStencilDesc;
		ID3D12PipelineState** PipelineState;
	};*/

	struct RendererCreationInfo
	{
		RenderAPIs RenderApi;
		s32 Width;
		s32 Height;
		bool bVSync;
		void* WndHandle;

		RendererCreationInfo()
			:RenderApi(RenderAPIs::DX12)
			, Width(-1)
			, Height(-1)
			, bVSync(false)
			, WndHandle(nullptr)
		{
		}

		RendererCreationInfo(RenderAPIs _renderApi, u32 _width, u32 _height, bool _bVsync, void* _wndHandle)
			:RenderApi(_renderApi)
			, Width(_width)
			, Height(_height)
			, bVSync(_bVsync)
			, WndHandle(_wndHandle)
		{
		}
	};

	//struct Vertex
	//{
	//	DirectX::XMFLOAT3 Position;
	//	DirectX::XMFLOAT3 Normal;
	//	DirectX::XMFLOAT2 TextureCoordinate;
	//};

	//struct Mesh
	//{
	//	Mesh() = default;

	//	Mesh(const Mesh& _other) = default;

	//	Mesh(Mesh&& _other) noexcept
	//		:Vertices(std::move(_other.Vertices))
	//		, Indices(std::move(_other.Indices))
	//	{
	//	}

	//	Mesh& operator=(const Mesh&) = default;

	//	std::vector<Vertex> Vertices;
	//	std::vector<u32> Indices;
	//};
}
