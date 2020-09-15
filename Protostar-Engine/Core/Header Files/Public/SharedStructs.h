#pragma once
#include "GlobalDefs.h"
#include <utility>
#include <vector>
#include "SharedEnums.h"
#include "DirectXMath.h"

namespace Protostar
{
	struct SPSOInfo
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDescs;
		ID3D12RootSignature* RootSignature;
		ID3DBlob* VertexShader;
		ID3DBlob* PixelShader;
		u32 NodeMask;
		D3D12_BLEND_DESC BlendDesc;
		D3D12_RASTERIZER_DESC RasterizerDesc;
		D3D12_DEPTH_STENCIL_DESC DepthStencilDesc;
		ID3D12PipelineState** PipelineState;
	};

	struct SRendererCreationInfo
	{
		ERenderAPIs RenderApi;
		u32 Width;
		u32 Height;
		bool bVSync;
		void* WndHandle;
	};

	struct SVertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TextureCoordinate;
	};

	struct SMesh
	{
		SMesh()
			: Vertices({})
			, Indices({})
		{

		}

		SMesh(const SMesh& _other)
			: Vertices(_other.Vertices)
			, Indices(_other.Indices)
		{

		}

		std::vector<SVertex> Vertices;
		std::vector<u32> Indices;
	};

	struct SWindowDimParams
	{
		SWindowDimParams() = default;

		SWindowDimParams(const SWindowDimParams& _other)
			: Width(_other.Width)
			, Height(_other.Height)
			, LeftTopCornerX(_other.LeftTopCornerX)
			, LeftTopCornerY(_other.LeftTopCornerY)
		{
		}

		SWindowDimParams(SWindowDimParams&& _other) noexcept
			: Width(std::move(_other.Width))
			, Height(std::move(_other.Height))
			, LeftTopCornerX(std::move(_other.LeftTopCornerX))
			, LeftTopCornerY(std::move(_other.LeftTopCornerY))
		{
		}

		void operator=(const SWindowDimParams& _other)
		{
			Width = _other.Width;
			Height = _other.Height;
			LeftTopCornerX = _other.LeftTopCornerX;
			LeftTopCornerY = _other.LeftTopCornerY;
		}

		s64 Width;
		s64 Height;
		s32 LeftTopCornerX;
		s32 LeftTopCornerY;
	};
}