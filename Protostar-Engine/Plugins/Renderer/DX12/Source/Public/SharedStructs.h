#ifndef SHAREDSTRUCTS_H
#define SHAREDSTRUCTS_H

#include <utility>
#include <vector>
#include <algorithm>

#include "GlobalDefinitions.h"
#include "SharedEnums.h"
#include "DirectXMath.h"
#include "d3d12.h"
#include "Json.h"
#include "Guid.h"

namespace Protostar
{
	struct PPSOInfo
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDescs;
		std::pair<PGuid, ID3D12RootSignature*> RootSignature;
		std::pair<PGuid, ID3DBlob*> VertexShader;
		std::pair<PGuid, ID3DBlob*> PixelShader;
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

		SRendererCreationInfo()
			:RenderApi(ERenderAPIs::DX12)
			,Width(-1)
			,Height(-1)
			,bVSync(false)
			,WndHandle(nullptr)
		{
		}

		SRendererCreationInfo(ERenderAPIs _renderApi, u32 _width, u32 _height, bool _bVsync, void* _wndHandle)
			:RenderApi(_renderApi)
			,Width(_width)
			,Height(_height)
			,bVSync(_bVsync)
			,WndHandle(_wndHandle)
		{
		}

		SRendererCreationInfo(const SRendererCreationInfo&) = default;

		SRendererCreationInfo(SRendererCreationInfo&& _other) noexcept
			:RenderApi(std::move(_other.RenderApi))
			,Width(std::move(_other.Width))
			,Height(std::move(_other.Height))
			,bVSync(std::move(_other.bVSync))
			,WndHandle(std::move(_other.WndHandle))
		{
			_other = SRendererCreationInfo();
		}

		SRendererCreationInfo& operator=(const SRendererCreationInfo& _other)
		{
			RenderApi = _other.RenderApi;
			Width = _other.Width;
			Height = _other.Height;
			bVSync = _other.bVSync;
			WndHandle = _other.WndHandle;
			return *this;
		}
	};

	struct SVertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TextureCoordinate;
	};

	struct SMesh
	{
		SMesh() = default;

		SMesh(const SMesh& _other) = default;

		SMesh(SMesh&& _other) noexcept
			:Vertices(std::move(_other.Vertices))
			,Indices(std::move(_other.Indices))
		{
		}

		SMesh& operator=(const SMesh&) = default;

		std::vector<SVertex> Vertices;
		std::vector<u32> Indices;
	};

	struct SWindowDimParams
	{
		SWindowDimParams()
			:Width(-1)
			,Height(-1)
			,LeftTopCornerX(-1)
			,LeftTopCornerY(-1)
		{
		};

		SWindowDimParams(const SWindowDimParams& _other) = default;

		SWindowDimParams(SWindowDimParams&& _other) noexcept
			: Width(std::move(_other.Width))
			, Height(std::move(_other.Height))
			, LeftTopCornerX(std::move(_other.LeftTopCornerX))
			, LeftTopCornerY(std::move(_other.LeftTopCornerY))
		{
			_other = SWindowDimParams();
		}

		SWindowDimParams& operator=(const SWindowDimParams& _other)
		{
			Width = _other.Width;
			Height = _other.Height;
			LeftTopCornerX = _other.LeftTopCornerX;
			LeftTopCornerY = _other.LeftTopCornerY;
			return *this;
		}

		s64 Width;
		s64 Height;
		s32 LeftTopCornerX;
		s32 LeftTopCornerY;
	};
}
#endif //SHAREDSTRUCTS_H